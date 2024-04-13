// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraphNode.h"
#include "QuestSystemGraphNode_Objective.generated.h"

class UQuestTask;

UCLASS(Blueprintable)
class QUEST_SYSTEM_RUNTIME_API UQuestSystemGraphNode_Objective : public UQuestSystemGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = "Objective")
		TArray<UQuestTask*> Tasks;
	

	UFUNCTION(BlueprintCallable, Category = "Objective")
		bool IsAllTaskCompleted();


#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;
	virtual void SetNodeTitle(const FText& NewTitle);
	virtual FText GetNodeDescription() const override;
#endif
};
