// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestEvent.h"
#include "QuestComponent.h"
#include "QuestSystem.h"
#include "QuestSystemGraph.h"


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
