// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/RestartQuest.h"
#include "QuestSystem.h"
#include "QuestComponent.h"
#include "QuestSystemGraph.h"
#include "QuestSystemGraphNode.h"
#include "QuestSystemGraphNode_Objective.h"

URestartQuest::URestartQuest()
{
	bUseQuestID = true;
}

void URestartQuest::BeginEvent_Implementation()
{
	GetQuestComponent()->RestartQuest(QuestID, NodeID, bStartFromNodeID);
}

