// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/FailQuest.h"
#include "QuestSystem.h"
#include "QuestComponent.h"
#include "QuestSystemGraph.h"
#include "QuestSystemGraphNode.h"

UFailQuest::UFailQuest()
{
	bUseQuestID = true;
}

void UFailQuest::BeginEvent_Implementation()
{
	if (!QuestID.IsNone())
	{
		GetQuestComponent()->FailQuest(QuestID);
	}
	
}
