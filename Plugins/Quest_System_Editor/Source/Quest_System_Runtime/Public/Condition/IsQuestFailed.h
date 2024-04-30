// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Condition/QuestCondition.h"
#include "IsQuestFailed.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_SYSTEM_RUNTIME_API UIsQuestFailed : public UQuestCondition
{
	GENERATED_BODY()

public:
	UIsQuestFailed();


	virtual bool IsConditionMet_Implementation() const override;
};
