// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemGraph.h"


#define LOCTEXT_NAMESPACE "QuestSystemGraph"




UQuestSystemGraph::UQuestSystemGraph()
{
	NodeType = UQuestSystemGraphNode::StaticClass();
	EdgeType = UQuestSystemGraphEdge::StaticClass();

	bEdgeEnabled = false;
#if WITH_EDITORONLY_DATA

	bCanRenameNode = true;
#endif
}

UQuestSystemGraph::~UQuestSystemGraph()
{
	
}



void UQuestSystemGraph::GetAllGraphs(TArray<UEdGraph*>& Graphs) const
{
#if WITH_EDITORONLY_DATA
	

	for (int32 i = 0; i < QuestGraphPages.Num(); ++i)
	{
		UEdGraph* Graph = QuestGraphPages[i];
		if (Graph)
		{
			Graphs.Add(Graph);
			Graph->GetAllChildrenGraphs(Graphs);
		}
	}
	
#endif // WITH_EDITORONLY_DATA
}

#undef LOCTEXT_NAMESPACE