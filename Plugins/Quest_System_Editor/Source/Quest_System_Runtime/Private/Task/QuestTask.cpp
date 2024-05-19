// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/QuestTask.h"
#include "QuestComponent.h"
#include "QuestSystem.h"
#include "QuestSystemGraph.h"
#include "QuestSystemGraphNode.h"

bool UQuestTask::CompleteTask()
{	
	if (QuestSystem == NULL)
        return false;

    TaskState = ETaskState::E_Complete;
    OnTaskCompleted();
	GetQuestComponent()->UpdateQuest();
    GetQuestComponent()->OnQuestObjectiveUpdated.Broadcast();
    
	return true;
}

AController* UQuestTask::GetOwningController() const
{
	if (!QuestGraph)
		return nullptr;

	return QuestGraph->OwningController;
}

UQuestComponent* UQuestTask::GetQuestComponent() const
{
	if (!QuestGraph)
		return nullptr;

	return QuestGraph->QuestComponent;
}

UQuestSystemGraph* UQuestTask::GetOwningQuestGraph() const
{
	if (!QuestGraph)
		return nullptr;

	return QuestGraph;
}

UQuestSystem* UQuestTask::GetOwningQuestSystem() const
{
	return QuestSystem;
}

void UQuestTask::AddAmount(int AmountToAdd)
{
	CurrentAmount = FMath::Clamp(CurrentAmount + AmountToAdd, 0, RequiredAmount);
	if (CurrentAmount >= RequiredAmount)
	{
		CompleteTask();
	}
}

FText UQuestTask::GetDescription_Implementation() const
{
	return Description;
}

void UQuestTask::Tick(float DeltaTime)
{
	if (LastFrameNumberWeTicked == GFrameCounter)
		return;
	//Do The Tick Logic
	TaskTick();
	LastFrameNumberWeTicked = GFrameCounter;
}
