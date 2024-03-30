// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemGraphNode_Root.h"
#include "QuestSystemGraphNode.h"


#if WITH_EDITOR
FText UQuestSystemGraphNode_Root::GetNodeDescription() const
{
	return FText::FromString(TEXT("Root Node"));
}
#endif