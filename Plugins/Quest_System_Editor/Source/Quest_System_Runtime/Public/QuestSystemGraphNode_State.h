// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraphNode.h"
#include "QuestSystem.h"
#include "QuestSystemGraphNode_State.generated.h"


UCLASS(/*HideCategories = ("Detail")*/)
class QUEST_SYSTEM_RUNTIME_API UQuestSystemGraphNode_State : public UQuestSystemGraphNode
{
	GENERATED_BODY()
public:

    UPROPERTY(BlueprintReadWrite, Category = "State")
        EQuestState QuestState;

    UFUNCTION(BlueprintCallable, Category = "State")
        void BeginState();

#if WITH_EDITOR
    virtual FText GetNodeTitle() const override;

    //virtual FText GetNodeDescription() const override;

    virtual FLinearColor GetBackgroundColor() const override;
#endif
};
