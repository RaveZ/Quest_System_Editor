// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include "QuestSystem.h"
#include "Condition/QuestCondition.h"
#include "Event/QuestEvent.h"
#include "Task/QuestTask.h"
#include "QuestSaveObject.h"
#include "QuestSystemGraph.h"
#include "QuestSystemGraphNode_State.h"
#include "QuestSystemGraphNode_Root.h"
#include "QuestSystemGraphNode_TaskManager.h"
#include "Kismet/GameplayStatics.h"



#define LOCTEXT_NAMESPACE "QuestComponent"
// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

bool UQuestComponent::RunQuest(UQuestSystemGraph* QuestGraph)
{
	if (QuestGraph == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("RunQuest: Unable to run NULL Quest Graph"));
		return false;
	}

	//Set Dependency
	OwningController = Cast<AController>(GetOwner());
	OwningPawn = OwningController->K2_GetPawn();
	QuestComponent = this;


	//Set Quest Asset Dependency
	QuestGraph->OwningController = OwningController;
	QuestGraph->OwningPawn = OwningPawn;
	QuestGraph->QuestComponent = QuestComponent;


	bool bSuccess = true;
	
	//Add All Quest To QuestSystemMap
	for (UQuestSystem* QuestSystem : QuestGraph->QuestSystems)
	{
		QuestSystemMap.Emplace(QuestSystem->QuestID, QuestSystem);
	}

	//Add QuestGraph to array
	QuestGraphs.Add(QuestGraph);

	//Begin the Quest
	for (UQuestSystem* QuestSystem : QuestGraph->QuestSystems)
	{
		for (UQuestSystemGraphNode* Node : QuestSystem->RootNodes)
		{
			if (QuestSystem->QuestState == EQuestState::E_Active)
			{
				RestartQuest(QuestSystem->QuestID);
			}
		}
	}

	return bSuccess;
}




void UQuestComponent::RestartQuest(FName QuestID, FName NodeID, bool bStartFromNodeID)
{
	if (UQuestSystem* QuestSystem = *QuestSystemMap.Find(QuestID))
	{
		QuestSystem->QuestState = EQuestState::E_Active;

		for (UQuestSystemGraphNode* Node : QuestSystem->AllNodes)
		{
			if (UQuestSystemGraphNode_TaskManager* TaskManagerNode = Cast<UQuestSystemGraphNode_TaskManager>(Node))
			{
				for (UQuestTask* Task : TaskManagerNode->Tasks)
				{
					Task->TaskState = ETaskState::E_Active;
					Task->CurrentAmount = 0;
				}
			}
		}
		if (bStartFromNodeID)
		{
			if (UQuestSystemGraphNode* FoundedNode = *QuestSystem->NodeMap.Find(NodeID))
			{
				QuestSystem->CurrentNode = FoundedNode;
			}
			else
			{
				QuestSystem->CurrentNode = QuestSystem->RootNodes[0];
			}
		}
		else
		{
			QuestSystem->CurrentNode = QuestSystem->RootNodes[0];
		}
		

		QuestSystem->VisitedNodeIDs.Empty();
		BeginNode(QuestSystem);
	}
}

void UQuestComponent::FailQuest(FName QuestID)
{
	if (UQuestSystem* QuestSystem = *QuestSystemMap.Find(QuestID))
	{
		if (QuestSystem->QuestState != EQuestState::E_Fail)
		{
			QuestSystem->QuestState = EQuestState::E_Fail;
			OnQuestStateUpdated.Broadcast(QuestSystem);
		}
		QuestSystem->CurrentNode = nullptr;

	}
}

void UQuestComponent::OnUpdateQuest(UQuestSystem* UpdatedQuest)
{
	for (const auto& It : QuestSystemMap)
	{
		FName Key = It.Key;
		UQuestSystem* QuestSystem = It.Value;

		if (UQuestSystemGraphNode_TaskManager* NodeTaskManager = Cast<UQuestSystemGraphNode_TaskManager>(QuestSystem->CurrentNode))
		{
			if (NodeTaskManager->IsAllTaskCompleted())
			{
				//evaluate next node condition
				EvaluateNextNodeCondition(QuestSystem);
			}
		}
	}
}

void UQuestComponent::EvaluateNextNodeCondition(UQuestSystem* QuestParam)
{
	UQuestSystemGraphNode* NextNode = nullptr;
	for (UQuestSystemGraphNode* Node : QuestParam->CurrentNode->ChildrenNodes)
	{
		if (NextNode == nullptr && Node->Conditions.IsEmpty())
		{
			NextNode = Node;
		}
		else if (!Node->Conditions.IsEmpty())
		{
			int SumConditionMet = 0;
			for (UQuestCondition* Condition : Node->Conditions)
			{
				if (Condition->IsConditionMet())
				{
					SumConditionMet++;
				}
			}
			if (SumConditionMet >= Node->Conditions.Num())
			{
				NextNode = Node;
				//break loop to ensure the met condition doesn't get overrided
				break;
			}
		}
	}

	if (NextNode)
	{
		//End of a Node, Launch Event with the End and Both Type
		for (UQuestEvent* Event : QuestParam->CurrentNode->Events)
		{
			if (Event->EventRunType == EEventRunType::E_Both || Event->EventRunType == EEventRunType::E_End)
			{
				Event->BeginEvent();
			}
		}
		//Go to next node if exist or condition met
		GoToNextNode(QuestParam, NextNode);

	}
	
}

void UQuestComponent::GoToNextNode(UQuestSystem* QuestParam, UQuestSystemGraphNode* NodeParam)
{
	QuestParam->CurrentNode = NodeParam;
	BeginNode(QuestParam);
}

void UQuestComponent::BeginNode(UQuestSystem* QuestParam)
{
	//add visited node
	QuestParam->VisitedNodeIDs.AddUnique(QuestParam->CurrentNode->ID);

	if (UQuestSystemGraphNode_Root* RootNode = Cast<UQuestSystemGraphNode_Root>(QuestParam->CurrentNode))
	{
		EvaluateNextNodeCondition(QuestParam);
		return;
	}

	//Start of the Node, Launch Event with the Start and Both Type
	for (UQuestEvent* Event : QuestParam->CurrentNode->Events)
	{
		if (Event->EventRunType == EEventRunType::E_Both || Event->EventRunType == EEventRunType::E_Start)
		{
			Event->BeginEvent();
		}
	}

	//Begin Node Functionality
	if (UQuestSystemGraphNode_TaskManager* TaskManagerNode = Cast<UQuestSystemGraphNode_TaskManager>(QuestParam->CurrentNode))
	{
		for (UQuestTask* Task : TaskManagerNode->Tasks)
		{
			Task->TaskBegin();

		}
	}

	if (UQuestSystemGraphNode_State* StateNode = Cast<UQuestSystemGraphNode_State>(QuestParam->CurrentNode))
	{
		StateNode->BeginState();
	}
}




void UQuestComponent::Save_Implementation(const FString& SaveFileName, int SlotIndex) const
{
	//Create New Save
	if (UQuestSaveObject* QuestSaveObject = Cast<UQuestSaveObject>(UGameplayStatics::CreateSaveGameObject(UQuestSaveObject::StaticClass())))
	{
		//Start Saving Data
		for (const auto& It : QuestSystemMap)
		{
			FName Key = It.Key;
			UQuestSystem* QuestSystem = It.Value;
				TMap<FName, FNodeData> NodeMapData;
				for (UQuestSystemGraphNode* Node : QuestSystem->AllNodes)
				{
					TArray<FTaskData> TaskDataArray;
					if (UQuestSystemGraphNode_TaskManager* TaskNode = Cast<UQuestSystemGraphNode_TaskManager>(Node))
					{
						for (UQuestTask* Task : TaskNode->Tasks)
						{
							FTaskData TaskData;
							TaskData.CurrentAmount = Task->CurrentAmount;
							TaskData.TaskState = Task->TaskState;
							TaskDataArray.Add(TaskData);
						}
					}
					FNodeData NodeData;
					NodeData.NodeID = Node->ID;
					NodeData.TaskDatas = TaskDataArray;
					NodeMapData.Emplace(Node->ID, NodeData);
				}
				FQuestData QuestData;
				QuestData.CurrentNodeID = QuestSystem->CurrentNode->ID;
				QuestData.QuestState = QuestSystem->QuestState;
				QuestData.NodeMapData = NodeMapData;
				QuestData.VisitedNodeIDs = QuestSystem->VisitedNodeIDs;
				QuestSaveObject->QuestMapData.Emplace(QuestSystem->QuestID, QuestData);
			
		}

		UGameplayStatics::AsyncSaveGameToSlot(QuestSaveObject, SaveFileName, SlotIndex);
	}
	


}

void UQuestComponent::Load_Implementation(const FString& SaveFileName, int SlotIndex) const
{
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	// USomeUObjectClass::LoadGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData
	LoadedDelegate.BindUObject(this, &UQuestComponent::LoadGameDelegateFunction);
	UGameplayStatics::AsyncLoadGameFromSlot(SaveFileName, SlotIndex, LoadedDelegate);
}

void UQuestComponent::LoadGameDelegateFunction(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData) const
{
	if (UQuestSaveObject* LoadedData = Cast<UQuestSaveObject>(LoadedGameData))
	{
		for (const auto& QuestIt : LoadedData->QuestMapData)
		{
			FName QuestID = QuestIt.Key;
			FQuestData QuestData = QuestIt.Value;

			UQuestSystem* FoundedQuest = *QuestSystemMap.Find(QuestID);
			if (FoundedQuest != nullptr)
			{
				//Fill Quest Data Here
				FoundedQuest->QuestState = QuestData.QuestState;
				FoundedQuest->CurrentNode = *FoundedQuest->NodeMap.Find(QuestData.CurrentNodeID);
				FoundedQuest->VisitedNodeIDs = QuestData.VisitedNodeIDs;

				for (const auto& NodeIt : QuestData.NodeMapData)
				{
					FName NodeID = NodeIt.Key;
					FNodeData NodeData = NodeIt.Value;
					
					UQuestSystemGraphNode* FoundedNode = *FoundedQuest->NodeMap.Find(NodeID);
					if (FoundedNode != nullptr)
					{
						//Fill Node Data Here
						if (UQuestSystemGraphNode_TaskManager* TaskManagerNode = Cast<UQuestSystemGraphNode_TaskManager>(FoundedNode))
						{
							for (int i = 0; i<TaskManagerNode->Tasks.Num(); i++)
							{
								if (i >= NodeData.TaskDatas.Num() - 1)
								{
									break;
								}
								else
								{
									//fill task data here
									TaskManagerNode->Tasks[i]->CurrentAmount = NodeData.TaskDatas[i].CurrentAmount;
									TaskManagerNode->Tasks[i]->TaskState = NodeData.TaskDatas[i].TaskState;
								}
							}
						}
					}
				}
			}
		}

	}
}

// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();
	QuestSystemMap.Empty();
	OnQuestObjectiveUpdated.AddDynamic(this, &UQuestComponent::OnUpdateQuest);
	// ...
	
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

#undef LOCTEXT_NAMESPACE