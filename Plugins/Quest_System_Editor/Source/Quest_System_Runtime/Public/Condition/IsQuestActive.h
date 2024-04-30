// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Condition/QuestCondition.h"
#include "IsQuestActive.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_SYSTEM_RUNTIME_API UIsQuestActive : public UQuestCondition
{
	GENERATED_BODY()

public:
	UIsQuestActive();

	virtual bool IsConditionMet_Implementation() const override;
};
