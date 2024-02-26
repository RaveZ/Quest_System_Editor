// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL_QuestSystem.h"
#include "QuestSystemGraphNode_Root.h"

#define LOCTEXT_NAMESPACE "BFL_QuestGraph"
UQuestSystemGraph* UBFL_QuestSystem::RunQuest(UQuestSystemGraph* QuestAsset)
{
	if (QuestAsset == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("RunQuest: Unable to run NULL Quest Graph"));
		return false;
	}

	bool bSuccess = true;
	UQuestSystemGraph* Quest = CastChecked<UQuestSystemGraph>(QuestAsset);

	for (UQuestSystemGraphNode* Node : Quest->RootNodes)
	{
		//TODO : pengecekan state quest dan state custom untuk menggantikan ENUM QUestState
		if (UQuestSystemGraphNode_Root* RootNode = Cast<UQuestSystemGraphNode_Root>(Node))
		{
			if (RootNode->QuestState == EQuestState::E_Active)
			{
				RootNode->GoToNextNode();

			}
				
		}
	}	
	
	

	return Cast<UQuestSystemGraph>(QuestAsset);
}

#undef LOCTEXT_NAMESPACE