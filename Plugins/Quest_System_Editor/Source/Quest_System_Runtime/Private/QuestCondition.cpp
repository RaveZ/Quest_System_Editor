// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestCondition.h"
#include "QuestComponent.h"
#include "QuestSystem.h"
#include "QuestSystemGraph.h"

#define LOCTEXT_NAMESPACE "QuestCondition"

bool UQuestCondition::IsConditionMet_Implementation() const
{
	return true;
}

AController* UQuestCondition::GetOwningController() const
{
	if (!QuestGraph)
		return nullptr;

	return QuestGraph->OwningController;
}

APawn* UQuestCondition::GetOwningPawn() const
{
	if (!QuestGraph)
		return nullptr;

	return QuestGraph->OwningPawn;
}

UQuestComponent* UQuestCondition::GetQuestComponent() const
{
	if (!QuestGraph)
		return nullptr;

	return QuestGraph->QuestComponent;
}

UQuestSystemGraph* UQuestCondition::GetOwningQuestGraph() const
{
	if (!QuestGraph)
		return nullptr;

	return QuestGraph;
}

UQuestSystem* UQuestCondition::GetOwningQuestSystem() const
{
	return QuestSystem;
}

#undef LOCTEXT_NAMESPACE