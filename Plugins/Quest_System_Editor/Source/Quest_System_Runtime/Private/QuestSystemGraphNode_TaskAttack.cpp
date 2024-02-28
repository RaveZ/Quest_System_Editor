// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemGraphNode_TaskAttack.h"


#define LOCTEXT_NAMESPACE "QuestSystemGraphNode_Task"
FText UQuestSystemGraphNode_TaskAttack::GetNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Task Attack");
}

#undef LOCTEXT_NAMESPACE