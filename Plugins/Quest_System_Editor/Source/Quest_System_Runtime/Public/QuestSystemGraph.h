// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraphNode.h"
#include "QuestSystemGraphEdge.h"
#include "GameplayTagContainer.h"
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
	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraph")
		TSubclassOf<UQuestSystemGraphNode> NodeType;
	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraph")
		TSubclassOf<UQuestSystemGraphEdge> EdgeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestSystemGraph")
		FGameplayTagContainer GraphTags;

	UPROPERTY(BlueprintReadOnly, Category = "QuestSystemGraph")
		TArray<UQuestSystemGraphNode*> RootNodes;

	UPROPERTY(BlueprintReadOnly, Category = "QuestSystemGraph")
		TArray<UQuestSystemGraphNode*> AllNodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestSystemGraph")
		bool bEdgeEnabled;

	UFUNCTION(BlueprintCallable, Category = "QuestSystemGraph")
		void Print(bool ToConsole = true, bool ToScreen = true);

	UFUNCTION(BlueprintCallable, Category = "QuestSystemGraph")
		int GetLevelNum() const;

	UFUNCTION(BlueprintCallable, Category = "QuestSystemGraph")
		void GetNodesByLevel(int Level, TArray<UQuestSystemGraphNode*>& Nodes);

	void ClearGraph();

#if WITH_EDITORONLY_DATA
	/** Set of pages that combine into a single uber-graph */
	UPROPERTY()
	TArray<TObjectPtr<UEdGraph>> QuestGraphPages;

	UPROPERTY()
		class UEdGraph* EdGraph;

	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraph")
		bool bCanRenameNode;

	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraph")
		bool bCanBeCyclical;
#endif
};
