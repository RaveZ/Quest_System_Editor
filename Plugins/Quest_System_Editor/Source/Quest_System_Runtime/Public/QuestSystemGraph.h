// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraph.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class QUEST_SYSTEM_RUNTIME_API UQuestSystemGraph : public UObject
{
	GENERATED_BODY()
public:
	UQuestSystemGraph();
	virtual ~UQuestSystemGraph();

	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraph")
		FString Name;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
		class UEdGraph* EdGraph;

	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraph")
		bool bCanRenameNode;

	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraph")
		bool bCanBeCyclical;
#endif
};
