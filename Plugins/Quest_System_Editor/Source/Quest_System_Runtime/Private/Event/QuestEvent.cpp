// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/QuestEvent.h"
#include "QuestComponent.h"
#include "QuestSystem.h"
#include "QuestSystemGraph.h"
#include "QuestSystemGraphNode.h"
#include "QuestSystemGraphNode_TaskManager.h"


UQuestEvent::UQuestEvent()
{
}

void UQuestEvent::BeginEvent_Implementation()
{
}

AController* UQuestEvent::GetOwningController() const
{
	if (!QuestGraph)
		return nullptr;

	return QuestGraph->OwningController;
}

APawn* UQuestEvent::GetOwningPawn() const
{
	if (!QuestGraph)
		return nullptr;

	return QuestGraph->OwningPawn;
}

UQuestComponent* UQuestEvent::GetQuestComponent() const
{
	if (!QuestGraph)
		return nullptr;

	return QuestGraph->QuestComponent;
}

UQuestSystemGraph* UQuestEvent::GetOwningQuestGraph() const
{
	if (!QuestGraph)
		return nullptr;

	return QuestGraph;
}

UQuestSystem* UQuestEvent::GetOwningQuestSystem() const 
{
	return QuestSystem;
}

TArray<FName> UQuestEvent::GetAllQuestID()
{
	TArray<FName> QuestIDs;
	for (TObjectIterator<UQuestSystem> It; It; ++It)
	{
		if (UQuestSystem* Quest = Cast<UQuestSystem>(*It))
		{
			QuestIDs.Add(Quest->QuestID);
		}
	}

	return QuestIDs;
}

