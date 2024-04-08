// Fill out your copyright notice in the Description page of Project Settings.


#include "Condition/IsQuestCompleted.h"
#include "QuestComponent.h"
#include "QuestSystem.h"

bool UIsQuestCompleted::IsConditionMet_Implementation() const
{
    if (UQuestSystem* FoundedQuest = *GetQuestComponent()->QuestSystemMap.Find(QuestID))
    {
        switch (FoundedQuest->QuestState)
        {
        case EQuestState::E_Active:
            return false;
        case EQuestState::E_Complete:
            return true;
        case EQuestState::E_Fail:
            return false;
        case EQuestState::E_Inactive:
            return false;
        default:
            return false;
        }
    }
    return false;
}
