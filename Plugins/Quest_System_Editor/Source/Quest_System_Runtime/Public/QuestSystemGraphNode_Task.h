// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraphNode.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystemGraphNode_Task.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_SYSTEM_RUNTIME_API UQuestSystemGraphNode_Task : public UQuestSystemGraphNode
{
	GENERATED_BODY()
public:
    UQuestSystemGraphNode_Task();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogueSession")
    FText Description;


#if WITH_EDITOR
    virtual FText GetNodeTitle() const override;

    virtual void SetNodeTitle(const FText& NewTitle) override;

    virtual FLinearColor GetBackgroundColor() const override;
#endif
};
