// Fill out your copyright notice in the Description page of Project Settings.


#include "Condition/QuestCondition.h"
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

TArray<FName> UQuestCondition::GetAllQuestID()
{
	TArray<FName> QuestIDs;
	for (TObjectIterator<UQuestSystemGraph> It; It; ++It)
	{
		for (UQuestSystem* Quest : It->QuestSystems)
		{
			QuestIDs.Add(Quest->QuestID);
		}
	}

	return QuestIDs;
}

#undef LOCTEXT_NAMESPACE