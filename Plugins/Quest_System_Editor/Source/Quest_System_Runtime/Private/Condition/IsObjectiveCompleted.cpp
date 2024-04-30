// Fill out your copyright notice in the Description page of Project Settings.


#include "Condition/IsObjectiveCompleted.h"
#include "QuestSystemGraphNode_Objective.h"
#include "QuestComponent.h"
#include "QuestSystem.h"

UIsObjectiveCompleted::UIsObjectiveCompleted()
{
}

bool UIsObjectiveCompleted::IsConditionMet_Implementation() const
{

    for (const auto& It : GetQuestComponent()->QuestSystemMap)
    {
        FName Key = It.Key;
        UQuestSystem* Quest = It.Value;

        if (UQuestSystemGraphNode_Objective* ObjectiveNode =  Cast<UQuestSystemGraphNode_Objective>(Quest->NodeMap.FindRef(NodeID)))
        {
            return ObjectiveNode->IsAllTaskCompleted();
        }
    }
    return false;
}
