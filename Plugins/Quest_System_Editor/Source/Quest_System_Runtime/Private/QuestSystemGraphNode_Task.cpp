// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemGraphNode_Task.h"
#include "QuestSystemGraphNode_TaskManager.h"
#include "QuestComponent.h"
#include "QuestSystem.h"
#include "QuestSystemGraph.h"
#include "QuestSystemGraphNode.h"

#define LOCTEXT_NAMESPACE "QuestSystemGraphNode_Task"

UQuestSystemGraphNode_Task::UQuestSystemGraphNode_Task()
{
}

bool UQuestSystemGraphNode_Task::CompleteTask()
{
   /* if (QuestSystem == NULL)
        return false;

    TaskState = ETaskState::E_Complete;
    OnTaskCompleted();
    GetQuestComponent()->OnQuestObjectiveUpdated.Broadcast(QuestSystem);
    */
    return true;
}



FText UQuestSystemGraphNode_Task::GetDescription_Implementation() const
{
    return Description;
}

AController* UQuestSystemGraphNode_Task::GetOwningController() const
{
    if (!QuestGraph)
        return nullptr;

    return QuestGraph->OwningController;
}

APawn* UQuestSystemGraphNode_Task::GetOwningPawn() const
{
    if (!QuestGraph)
        return nullptr;

    return QuestGraph->OwningPawn;
}

UQuestComponent* UQuestSystemGraphNode_Task::GetQuestComponent() const
{
    if (!QuestGraph)
        return nullptr;

    return QuestGraph->QuestComponent;
}

UQuestSystemGraph* UQuestSystemGraphNode_Task::GetOwningQuestGraph() const
{
    if (!QuestGraph)
        return nullptr;

    return QuestGraph;
}

UQuestSystem* UQuestSystemGraphNode_Task::GetOwningQuestSystem() const
{
    return QuestSystem;
}

void UQuestSystemGraphNode_Task::Tick(float DeltaTime)
{
    if (LastFrameNumberWeTicked == GFrameCounter)
        return;
    //Do The Tick Logic
    OnTaskTick();
    LastFrameNumberWeTicked = GFrameCounter;
}



#undef LOCTEXT_NAMESPACE