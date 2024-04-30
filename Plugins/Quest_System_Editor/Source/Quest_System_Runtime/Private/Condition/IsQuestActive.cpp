// Fill out your copyright notice in the Description page of Project Settings.


#include "Condition/IsQuestActive.h"
#include "QuestComponent.h"
#include "QuestSystem.h"

UIsQuestActive::UIsQuestActive()
{
    bUseQuestID = true;
}

bool UIsQuestActive::IsConditionMet_Implementation() const
{
    if (UQuestSystem* FoundedQuest = GetQuestComponent()->QuestSystemMap.FindRef(QuestID))
    {
        switch (FoundedQuest->QuestState)
        {
            case EQuestState::E_Active :
                return true;
            default:
                return false;
        }
    }
    return false;
}
