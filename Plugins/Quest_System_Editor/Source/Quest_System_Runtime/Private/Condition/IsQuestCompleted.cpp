// Fill out your copyright notice in the Description page of Project Settings.


#include "Condition/IsQuestCompleted.h"
#include "QuestComponent.h"
#include "QuestSystem.h"

bool UIsQuestCompleted::IsConditionMet_Implementation() const
{
    if (UQuestSystem* FoundedQuest = GetQuestComponent()->QuestSystemMap.FindRef(QuestID))
    {
        switch (FoundedQuest->QuestState)
        {
        case EQuestState::E_Complete:
            return true;
        default:
            return false;
        }
    }
    return false;
}
