// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/JumpToNextNode.h"
#include "QuestSystemGraphNode.h"
#include "QuestSystemGraph.h"
#include "QuestSystem.h"
#include "QuestComponent.h"

UJumpToNextNode::UJumpToNextNode()
{
}

TArray<FName> UJumpToNextNode::GetNextNodeID()
{
	TArray<FName> NodeIDs;
	if(OwningNode)
	{
		for (UQuestSystemGraphNode* Node : OwningNode->ChildrenNodes)
		{
			if (Node)
			{
				NodeIDs.Emplace(Node->ID);
			}
		}
	}
	
	return NodeIDs;
}

void UJumpToNextNode::BeginEvent_Implementation()
{
	if (UQuestSystemGraphNode* FoundedNode = GetOwningQuestSystem()->NodeMap.FindRef(NodeID))
	{
		GetQuestComponent()->GoToNextNode(GetOwningQuestSystem(), FoundedNode);
	}
}
