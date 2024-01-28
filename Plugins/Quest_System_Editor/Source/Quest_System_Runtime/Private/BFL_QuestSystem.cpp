// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL_QuestSystem.h"

UQuestSystemGraph* UBFL_QuestSystem::RunQuest(UQuestSystemGraph* _quest)
{
	return Cast<UQuestSystemGraph>(_quest);
}
