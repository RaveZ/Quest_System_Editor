// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL_QuestSystem.h"
#include "QuestSystem.h"
#include "QuestSystemGraphNode_Root.h"

#define LOCTEXT_NAMESPACE "BFL_QuestGraph"
bool UBFL_QuestSystem::RunQuest(UQuestSystemGraph* QuestAsset)
{
	if (QuestAsset == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("RunQuest: Unable to run NULL Quest Graph"));
		return false;
	}

	bool bSuccess = true;
	UQuestSystemGraph* QuestSystemGraph = CastChecked<UQuestSystemGraph>(QuestAsset);
	for (UQuestSystem* Quest : QuestSystemGraph->QuestSystems)
	{
		for (UQuestSystemGraphNode* Node : Quest->RootNodes)
		{
			//TODO : pengecekan state quest dan state custom untuk menggantikan ENUM QUestState
			if (Quest->QuestState == EQuestState::E_Active)
			{
				Quest->CurrentNode = Node;
				Node->GoToNextNode();
			}
		}
	}
	

	return bSuccess;
}



#undef LOCTEXT_NAMESPACE