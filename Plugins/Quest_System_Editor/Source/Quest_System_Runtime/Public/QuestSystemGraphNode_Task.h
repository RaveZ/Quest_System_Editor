// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraphNode.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystemGraphNode_Task.generated.h"

UENUM(BlueprintType)
enum class EClearCondition : uint8
{
    E_Default		UMETA(DisplayName = "DEFAULT"),
    E_Slay			UMETA(DisplayName = "SLAY"),
    E_Collect		UMETA(DisplayName = "COLLECT"),
    E_Travel		UMETA(DisplayName = "TRAVEL"),
};

UCLASS()
class QUEST_SYSTEM_RUNTIME_API UQuestSystemGraphNode_Task : public UQuestSystemGraphNode
{
	GENERATED_BODY()
public:
    UQuestSystemGraphNode_Task();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tasks")
        FText Description;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Tasks")
        void OnTaskBegin(AController* OwnerController);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Tasks")
        void OnTaskCompleted(AController* OwnerController);

    UFUNCTION(BlueprintCallable, Category = "Tasks")
        void CompleteTask();

#if WITH_EDITOR
    virtual FText GetNodeTitle() const override;

    virtual void SetNodeTitle(const FText& NewTitle) override;

    virtual FLinearColor GetBackgroundColor() const override;
#endif
};
