// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemGraphNode_TaskManager.h"
#include "QuestSystemGraphNode_Task.h"
#include "QuestSystem.h"
#include "QuestComponent.h"


FText UQuestSystemGraphNode_TaskManager::GetNodeTitle() const
{
    return ID.IsEmpty() ? FText::FromString(UQuestSystemGraphNode::GetShortTypeName(this)) : ID;
}

void UQuestSystemGraphNode_TaskManager::SetNodeTitle(const FText& NewTitle)
{
    ID = NewTitle;
}

FText UQuestSystemGraphNode_TaskManager::GetNodeDescription() const
{
    if (Tasks.Num() == 0)
    {
        return FText::FromString(TEXT("Task Node"));
    }

    FString BulletedList;

    for (const UQuestSystemGraphNode_Task* Task : Tasks)
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
