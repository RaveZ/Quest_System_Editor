// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraphNode_Task.h"
#include "QuestSystemGraphNode_TaskAttack.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_SYSTEM_RUNTIME_API UQuestSystemGraphNode_TaskAttack : public UQuestSystemGraphNode_Task
{
	GENERATED_BODY()
public:



#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;

	//virtual void SetNodeTitle(const FText& NewTitle) override;

	//virtual FLinearColor GetBackgroundColor() const override;
#endif
	
};
