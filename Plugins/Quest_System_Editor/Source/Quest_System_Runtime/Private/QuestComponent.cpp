// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include "QuestSystem.h"
#include "QuestCondition.h"
#include "QuestEvent.h"
#include "QuestTask.h"
#include "QuestSaveObject.h"
#include "QuestSystemGraph.h"
#include "QuestSystemGraphNode_State.h"
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

	QuestGraphs.Add(QuestGraph);

	bool bSuccess = true;
	for (UQuestSystem* QuestSystem : QuestGraph->QuestSystems)
	{
		for (UQuestSystemGraphNode* Node : QuestSystem->RootNodes)
		{
			if (QuestSystem->QuestState == EQuestState::E_Active)
			{
				if (QuestSystem->CurrentNode != nullptr)
				{
					//TODO : Restart quest ganti dengan start quest, termasuk cek save data 
					BeginNode(QuestSystem);
				}
				else
				{
					RestartQuest(QuestSystem);
				}
			}
		}

	}

	return bSuccess;
}

void UQuestComponent::StartQuest(UQuestSystem* QuestParam)
{
}

void UQuestComponent::RestartQuest(UQuestSystem* QuestParam)
{
	QuestParam->CurrentNode = QuestParam->RootNodes[0];
	EvaluateNextNodeCondition(QuestParam);
}

void UQuestComponent::FailQuest(UQuestSystem* QuestParam)
{
}

void UQuestComponent::OnUpdateQuest(UQuestSystem* UpdatedQuest)
{
	for (UQuestSystemGraph* QuestGraph : QuestGraphs)
	{
		for (UQuestSystem* QuestSystem : QuestGraph->QuestSystems)
		{
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

UQuestSystem*  UQuestComponent::GetQuestByID(FName QuestID)
{
	UQuestSystem* FoundedQuest = nullptr;
	for (UQuestSystemGraph* QuestGraph : QuestGraphs)
	{
		for (UQuestSystem* QuestSystem : QuestGraph->QuestSystems)
		{
			if (QuestSystem->QuestID == QuestID)
			{
				FoundedQuest = QuestSystem;
				break;
			}
		}
	}

	return FoundedQuest;
}


void UQuestComponent::Save_Implementation(const FString& SaveFileName, int SlotIndex) const
{
	//Create New Save
	if (UQuestSaveObject* QuestSaveObject = Cast<UQuestSaveObject>(UGameplayStatics::CreateSaveGameObject(UQuestSaveObject::StaticClass())))
	{
		//Start Saving Data
		for (UQuestSystemGraph* QuestGraph : QuestGraphs)
		{
			for (UQuestSystem* QuestSystem : QuestGraph->QuestSystems)
			{
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
					NodeMapData.Add(Node->ID, NodeData);
				}
				FQuestData QuestData;
				QuestData.QuestID = QuestSystem->QuestID;
				QuestData.NodeMapData = NodeMapData;
				QuestSaveObject->QuestDatas.Add(QuestData);
			}
		}

		UGameplayStatics::AsyncSaveGameToSlot(QuestSaveObject, SaveFileName, SlotIndex);
	}
	


}

void UQuestComponent::Load_Implementation(const FString& SaveFileName, int SlotIndex) const
{
}

// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();
	QuestGraphs.Empty();
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