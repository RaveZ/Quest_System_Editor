// Fill out your copyright notice in the Description page of Project Settings.


#include "EdNode_QuestSystemState.h"
#include "QuestSystemGraphNode.h"

UEdNode_QuestSystemState::UEdNode_QuestSystemState()
{
}

void UEdNode_QuestSystemState::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNode", TEXT("In"));
}

FText UEdNode_QuestSystemState::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	const UQuestSystemGraphNode* MyNode = Cast<UQuestSystemGraphNode>(QuestSystemGraphNode);
	if (MyNode != NULL)
	{
		return MyNode->GetNodeTitle();
	}

	return Super::GetNodeTitle(TitleType);
}
