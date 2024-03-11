// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraphNode.h"
#include "QuestSystemGraphNode_TaskManager.generated.h"

class UQuestSystemGraphNode_Task;

UCLASS(Blueprintable)
class QUEST_SYSTEM_RUNTIME_API UQuestSystemGraphNode_TaskManager : public UQuestSystemGraphNode
{
	GENERATED_BODY()

public:
	/** @return Node Title */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detail")
		FText ID;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly)
		TArray<UQuestSystemGraphNode_Task*> Tasks;
	
	void UpdateTaskProgress();


#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;
	virtual void SetNodeTitle(const FText& NewTitle);
	virtual FText GetNodeDescription() const override;
#endif
};
