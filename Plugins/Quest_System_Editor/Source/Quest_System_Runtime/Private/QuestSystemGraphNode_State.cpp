// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemGraphNode_State.h"
#include "QuestComponent.h"
#include "QuestSystem.h"


void UQuestSystemGraphNode_State::BeginState()
{
    QuestSystem->QuestState = QuestState;
    GetQuestComponent()->UpdateQuest();

    switch (QuestState)
    {
        case EQuestState::E_Complete:
            GetQuestComponent()->OnQuestCompleted.Broadcast(QuestSystem);
        case EQuestState::E_Fail:
            GetQuestComponent()->OnQuestFailed.Broadcast(QuestSystem);
    }
}

#if WITH_EDITOR
FText UQuestSystemGraphNode_State::GetNodeTitle() const
{
    switch (QuestState)
    {
    case EQuestState::E_Complete:
        return FText::FromString(TEXT("Complete Quest"));
    case EQuestState::E_Fail:
        return FText::FromString(TEXT("Fail Quest"));
    default:
        return FText::FromString(TEXT("Default State"));
    }
}


FLinearColor UQuestSystemGraphNode_State::GetBackgroundColor() const
{
    switch (QuestState)
    {
        case EQuestState::E_Complete:
            return FLinearColor::Green;
        case EQuestState::E_Fail:
            return FLinearColor::Red;
        default:
            return FLinearColor::Black;
    }
}
#endif