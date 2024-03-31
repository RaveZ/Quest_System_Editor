// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTask.h"
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
    GetQuestComponent()->OnQuestObjectiveUpdated.Broadcast(QuestSystem);
    
	return true;
}

AController* UQuestTask::GetOwningController() const
{
	if (!QuestGraph)
		return nullptr;

	return QuestGraph->OwningController;
}

APawn* UQuestTask::GetOwningPawn() const
{
	if (!QuestGraph)
		return nullptr;

	return QuestGraph->OwningPawn;
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
