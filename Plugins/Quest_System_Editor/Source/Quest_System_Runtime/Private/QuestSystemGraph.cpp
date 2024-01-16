// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemGraph.h"


#define LOCTEXT_NAMESPACE "QuestSystemGraph"

UQuestSystemGraph::UQuestSystemGraph()
{
	NodeType = UQuestSystemGraphNode::StaticClass();
	EdgeType = UQuestSystemGraphEdge::StaticClass();

	bEdgeEnabled = true;
#if WITH_EDITORONLY_DATA
	EdGraph = nullptr;

	bCanRenameNode = true;
#endif
}

UQuestSystemGraph::~UQuestSystemGraph()
{
	
}

void UQuestSystemGraph::Print(bool ToConsole, bool ToScreen)
{
	int Level = 0;
	TArray<UQuestSystemGraphNode*> CurrLevelNodes = RootNodes;
	TArray<UQuestSystemGraphNode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UQuestSystemGraphNode* Node = CurrLevelNodes[i];
			check(Node != nullptr);

			FString Message = FString::Printf(TEXT("%s, Level %d"), *Node->GetDescription().ToString(), Level);

			if (ToConsole)
			{
				//LOG_INFO(TEXT("%s"), *Message);
			}

			if (ToScreen && GEngine != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, Message);
			}

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}
}

int UQuestSystemGraph::GetLevelNum() const
{
	int Level = 0;
	TArray<UQuestSystemGraphNode*> CurrLevelNodes = RootNodes;
	TArray<UQuestSystemGraphNode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UQuestSystemGraphNode* Node = CurrLevelNodes[i];
			check(Node != nullptr);

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}

	return Level;
}

void UQuestSystemGraph::GetNodesByLevel(int Level, TArray<UQuestSystemGraphNode*>& Nodes)
{
	int CurrLEvel = 0;
	TArray<UQuestSystemGraphNode*> NextLevelNodes;

	Nodes = RootNodes;

	while (Nodes.Num() != 0)
	{
		if (CurrLEvel == Level)
			break;

		for (int i = 0; i < Nodes.Num(); ++i)
		{
			UQuestSystemGraphNode* Node = Nodes[i];
			check(Node != nullptr);

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		Nodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++CurrLEvel;
	}
}

void UQuestSystemGraph::ClearGraph()
{
	for (int i = 0; i < AllNodes.Num(); ++i)
	{
		UQuestSystemGraphNode* Node = AllNodes[i];
		if (Node)
		{
			Node->ParentNodes.Empty();
			Node->ChildrenNodes.Empty();
			Node->Edges.Empty();
		}
	}

	AllNodes.Empty();
	RootNodes.Empty();
}

#undef LOCTEXT_NAMESPACE