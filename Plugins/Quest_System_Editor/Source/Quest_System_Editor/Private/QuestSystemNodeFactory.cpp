// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestSystemNodeFactory.h"
#include <EdGraph/EdGraphNode.h>
#include "SGraphNode_QuestSystemEdge.h"
#include "SGraphNode_QuestSystemNode.h"
#include "EdNode_QuestSystemNode.h"
#include "EdNode_QuestSystemEdge.h"


TSharedPtr<class SGraphNode> FQuestSystemNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (UEdNode_QuestSystemNode* EdNode_GraphNode = Cast<UEdNode_QuestSystemNode>(Node))
	{
		return SNew(SGraphNode_QuestSystemNode, EdNode_GraphNode);
	}
	/*else if (UEdNode_QuestSystemEdge* EdNode_Edge = Cast<UEdNode_QuestSystemEdge>(Node))
	{
		return SNew(SGraphNode_QuestSystemEdge, EdNode_Edge);
	}*/
	return nullptr;
}
