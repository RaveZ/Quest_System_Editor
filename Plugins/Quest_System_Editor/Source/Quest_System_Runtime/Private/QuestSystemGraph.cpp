// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemGraph.h"

UQuestSystemGraph::UQuestSystemGraph()
{
#if WITH_EDITORONLY_DATA
	EdGraph = nullptr;

	bCanRenameNode = true;
#endif
}

UQuestSystemGraph::~UQuestSystemGraph()
{
}
