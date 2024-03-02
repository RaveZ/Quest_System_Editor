// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem.h"
#include "QuestSystemGraph.h"
#include "QuestSystemGraphNode_Task.h"


#define LOCTEXT_NAMESPACE "QuestSystem"

void UQuestSystem::Print(bool ToConsole, bool ToScreen)
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

void UQuestSystem::GoToNextNode()
{
	if (QuestGraph == NULL)
		return;
	if (CurrentNode == NULL)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Current node name: %s"), *CurrentNode->NodeTitle.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Current Task Owning Controller name: %s"), *CurrentNode->GetOwningController()->GetFName().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Current Task Owning Pawn name: %s"), *CurrentNode->GetOwningPawn()->GetFName().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Current Task Owning Component name: %s"), *CurrentNode->GetOwningComponent()->GetFName().ToString());


	if (CurrentNode->ChildrenNodes.Num() > 0)
	{
		for (UQuestSystemGraphNode* Node : CurrentNode->ChildrenNodes)
		{
			//TODO: Make them able to detect the children node AND Check Condition feature
			CurrentNode = Node;
			if (UQuestSystemGraphNode_Task* TaskNode = Cast<UQuestSystemGraphNode_Task>(CurrentNode))
			{
				TaskNode->OnTaskBegin(GetOwningController());
			}
		}
		//GoToNextNode();
	}
}

int UQuestSystem::GetLevelNum() const
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

void UQuestSystem::GetNodesByLevel(int Level, TArray<UQuestSystemGraphNode*>& Nodes)
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

AController* UQuestSystem::GetOwningController()
{
	return QuestGraph->OwningController;
}

APawn* UQuestSystem::GetOwningPawn()
{
	return QuestGraph->OwningPawn;
}

UActorComponent* UQuestSystem::GetOwningComponent()
{
	return QuestGraph->OwningComponent;
}

UQuestSystemGraph* UQuestSystem::GetOwningQuest() const
{
	return QuestGraph;
}

void UQuestSystem::ClearGraph()
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