// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph_QuestSystemGraph.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_SYSTEM_EDITOR_API UEdGraph_QuestSystemGraph : public UEdGraph
{
	GENERATED_BODY()
public:
	UEdGraph_QuestSystemGraph();
	virtual ~UEdGraph_QuestSystemGraph();

	virtual void RebuildQuestSystemGraph();
};
