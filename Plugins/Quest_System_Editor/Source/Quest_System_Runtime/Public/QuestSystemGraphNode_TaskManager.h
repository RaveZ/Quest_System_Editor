// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraphNode.h"
#include "QuestSystemGraphNode_TaskManager.generated.h"

class UQuestTask;

UCLASS(Blueprintable)
class QUEST_SYSTEM_RUNTIME_API UQuestSystemGraphNode_TaskManager : public UQuestSystemGraphNode
{
	GENERATED_BODY()

public:
	


	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = "TaskManager")
		TArray<UQuestTask*> Tasks;
	

	UFUNCTION(BlueprintCallable, Category = "TaskManager")
		bool IsAllTaskCompleted();


#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;
	virtual void SetNodeTitle(const FText& NewTitle);
	virtual FText GetNodeDescription() const override;
#endif
};
