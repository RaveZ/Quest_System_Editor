// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemGraphNode_TaskManager.h"
#include "QuestSystemGraphNode_Task.h"
#include "QuestTask.h"
#include "QuestSystem.h"
#include "QuestComponent.h"


#define LOCTEXT_NAMESPACE "QuestSystemGraphNode_TaskManager"

bool UQuestSystemGraphNode_TaskManager::IsAllTaskCompleted()
{
    int progress = 0;
    for (UQuestTask* NodeTask : Tasks)
    {
        if (NodeTask->TaskState == ETaskState::E_Complete)
        {
            progress++;
        }
    }

    if (progress >= Tasks.Num())
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

#if WITH_EDITOR
FText UQuestSystemGraphNode_TaskManager::GetNodeTitle() const
{
    return FText::FromName(TaskID).IsEmpty() ? FText::FromString(UQuestSystemGraphNode::GetShortTypeName(this)) : FText::FromName(TaskID);
}

void UQuestSystemGraphNode_TaskManager::SetNodeTitle(const FText& NewTitle)
{
    TaskID = FName(NewTitle.ToString());
}

FText UQuestSystemGraphNode_TaskManager::GetNodeDescription() const
{
    if (Tasks.Num() == 0)
    {
        return FText::FromString(TEXT("Task Node"));
    }

    FString BulletedList;

    for (const UQuestTask* Task : Tasks)
    {
        if (Task)
        {
            BulletedList += TEXT("• ");
            BulletedList += Task->GetDescription().ToString();
            BulletedList += TEXT("\n");
        }
        
    }

    return FText::FromString(BulletedList);
}
#endif

#undef LOCTEXT_NAMESPACE