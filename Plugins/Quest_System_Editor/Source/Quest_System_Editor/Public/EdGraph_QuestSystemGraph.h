// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph_QuestSystemGraph.generated.h"

class UQuestSystemGraph;
class UQuestSystemGraphNode;
class UQuestSystemGraphEdge;
class UEdNode_QuestSystemNode;
class UEdNode_QuestSystemEdge;

UCLASS()
class QUEST_SYSTEM_EDITOR_API UEdGraph_QuestSystemGraph : public UEdGraph
{
	GENERATED_BODY()
public:
	UEdGraph_QuestSystemGraph();
	virtual ~UEdGraph_QuestSystemGraph();

	virtual void RebuildQuestSystemGraph();

	UQuestSystemGraph* GetQuestSystemGraph() const;

	virtual bool Modify(bool bAlwaysMarkDirty = true) override;
	virtual void PostEditUndo() override;

	UPROPERTY(Transient)
		TMap<UQuestSystemGraphNode*, UEdNode_QuestSystemNode*> NodeMap;

	UPROPERTY(Transient)
		TMap<UQuestSystemGraphEdge*, UEdNode_QuestSystemEdge*> EdgeMap;


		/** All UQuestGraphNode for this EdGraph */
	UPROPERTY(Transient)
		TArray<UQuestSystemGraphNode*> EdGraphAllNodes;

protected:
	void Clear();

	void SortNodes(UQuestSystemGraphNode* RootNode);
};
