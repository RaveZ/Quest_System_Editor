// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Event/QuestEvent.h"
#include "FailQuest.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_SYSTEM_RUNTIME_API UFailQuest : public UQuestEvent
{
	GENERATED_BODY()
public:
	UFailQuest();

	virtual void BeginEvent_Implementation() override;
};
