// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/RestartQuest.h"
#include "QuestSystem.h"
#include "QuestComponent.h"
#include "QuestSystemGraph.h"
#include "QuestSystemGraphNode.h"
#include "QuestSystemGraphNode_Objective.h"

URestartQuest::URestartQuest()
{
}

void URestartQuest::BeginEvent_Implementation()
{
	GetQuestComponent()->RestartQuest(QuestID, NodeID, bStartFromNodeID);
}

TArray<FName> URestartQuest::GetAllNodeID()
{
	TArray<FName> NodeIDs;
	if (!QuestID.IsNone())
	{
		for (TObjectIterator<UQuestSystem> It; It; ++It)
		{
			if (UQuestSystem* Quest = Cast<UQuestSystem>(*It))
			{
				if (Quest->QuestID == QuestID)
				{
					for (UQuestSystemGraphNode* QuestNode : Quest->AllNodes)
					{
						if (UQuestSystemGraphNode_Objective* ObjectiveNode = Cast<UQuestSystemGraphNode_Objective>(QuestNode))
						{
							NodeIDs.Add(QuestNode->ID);
						}
					}
					break;
				}

			}
		}
	}
	else
	{
		NodeID = NAME_None;
	}

	return NodeIDs;
}
