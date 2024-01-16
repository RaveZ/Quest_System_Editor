// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "UObject/NoExportTypes.h"
#include "EdGraph_QuestSystemGraph.h"
#include "QuestSystemGraph.h"
#include "EdNode_QuestSystemNode.generated.h"

class UEdNode_QuestSystemEdge;
class UEdGraph_QuestSystemGraph;
class SGraphNode_QuestSystemNode;

UCLASS()
class QUEST_SYSTEM_EDITOR_API UEdNode_QuestSystemNode : public UEdGraphNode
{
	GENERATED_BODY()
public:
	UEdNode_QuestSystemNode();
	virtual ~UEdNode_QuestSystemNode();

	UPROPERTY(VisibleAnywhere, Instanced, Category = "GenericGraph")
		UQuestSystemGraphNode* QuestSystemGraphNode;

	void SetQuestSystemGraphNode(UQuestSystemGraphNode* InNode);
	UEdGraph_QuestSystemGraph* GetQuestSystemGraphEdGraph();

	SGraphNode_QuestSystemNode* SGraphNode;

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PrepareForCopying() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;

	virtual FLinearColor GetBackgroundColor() const;
	virtual UEdGraphPin* GetInputPin() const;
	virtual UEdGraphPin* GetOutputPin() const;

#if WITH_EDITOR
	virtual void PostEditUndo() override;
#endif
};
