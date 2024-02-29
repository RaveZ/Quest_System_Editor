// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraphNode.h"
#include "QuestSystem.h"
#include "QuestSystemGraphNode_Root.h"
#include "QuestSystemGraphEdge.h"
#include "GameplayTagContainer.h"
#include "QuestSystemGraph.generated.h"



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


	/** Set of QuestSystem linked with Quest Graph Pages */
	UPROPERTY(BlueprintReadOnly, Category = "QuestSystemGraph")
		TArray<UQuestSystem*> QuestSystems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestSystemGraph")
		bool bEdgeEnabled;

	UPROPERTY(BlueprintReadWrite, Category = "QuestSystemGraph")
		AController* OwningController;

	UPROPERTY(BlueprintReadWrite, Category = "QuestSystemGraph")
		APawn* OwningPawn;

	UPROPERTY(BlueprintReadWrite, Category = "QuestSystemGraph")
		UActorComponent* OwningComponent;


#if WITH_EDITORONLY_DATA
	/** Set of quest-graph pages */
	UPROPERTY()
	TArray<TObjectPtr<UEdGraph>> QuestGraphPages;


	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraph")
		bool bCanRenameNode;

	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraph")
		bool bCanBeCyclical;

	/** Set of documents that were being edited in this blueprint, so we can open them right away */
	UPROPERTY()
	TArray<struct FEditedDocumentInfo> LastEditedDocuments;

	/** Whether or not this blueprint is newly created, and hasn't been opened in an editor yet */
	UPROPERTY(/*transient*/ BlueprintReadOnly, Category = "QuestSystemGraph")
	bool bIsNewlyCreated = true;
#endif

	/** Get all graphs in this QuestEditor */
	void GetAllGraphs(TArray<UEdGraph*>& Graphs) const;
};
