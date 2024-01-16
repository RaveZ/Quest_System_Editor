// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemGraphEdge.h"

UQuestSystemGraphEdge::UQuestSystemGraphEdge()
{
}

UQuestSystemGraphEdge::~UQuestSystemGraphEdge()
{
}

UQuestSystemGraph* UQuestSystemGraphEdge::GetGraph() const
{
	return Graph;
}


#if WITH_EDITOR
void UQuestSystemGraphEdge::SetNodeTitle(const FText& NewTitle)
{
	NodeTitle = NewTitle;
}
#endif // #if WITH_EDITOR

