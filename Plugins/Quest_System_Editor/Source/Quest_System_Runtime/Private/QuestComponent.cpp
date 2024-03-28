// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include "QuestSystem.h"
#include "QuestCondition.h"
#include "QuestSystemGraph.h"
#include "QuestSystemGraphNode_State.h"
#include "QuestSystemGraphNode_Task.h"
#include "QuestSystemGraphNode_TaskManager.h"



#define LOCTEXT_NAMESPACE "QuestComponent"
// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

bool UQuestComponent::StartQuest(UQuestSystemGraph* QuestGraph)
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
			//TODO : pengecekan state quest dan state custom untuk menggantikan ENUM QuestState
			if (QuestSystem->QuestState == EQuestState::E_Active)
			{
				QuestSystem->CurrentNode = Node;
				QuestSystem->GoToNextNode();
			}
		}

	}

	return bSuccess;
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
					//launch node events


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
		//Go to next node if exist or condition met
		GoToNextNode(QuestParam, NextNode);
	}
	
}

void UQuestComponent::GoToNextNode(UQuestSystem* QuestParam, UQuestSystemGraphNode* NodeParam)
{
	QuestParam->CurrentNode = NodeParam;
	if (UQuestSystemGraphNode_TaskManager* TaskManagerNode = Cast<UQuestSystemGraphNode_TaskManager>(QuestParam->CurrentNode))
	{
		for (UQuestSystemGraphNode_Task* TaskNode : TaskManagerNode->Tasks)
		{
			TaskNode->OnTaskBegin(OwningController);
		}
	}

	if (UQuestSystemGraphNode_State* StateNode = Cast<UQuestSystemGraphNode_State>(QuestParam->CurrentNode))
	{
		StateNode->BeginState();
	}
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