// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraphNode.h"
#include "Tickable.h"
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


UCLASS(Blueprintable, EditInlineNew, HideDropdown)
class QUEST_SYSTEM_RUNTIME_API UQuestSystemGraphNode_Task : public UQuestSystemGraphNode, public FTickableGameObject
{
	GENERATED_BODY()
public:
    UQuestSystemGraphNode_Task();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tasks")
        FText Description;

    UPROPERTY(Instanced, EditAnywhere, Category = "Tasks")
        TArray<UQuestSystemGraphNode_Task*> Tasks;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Tasks")
        void OnTaskBegin(AController* OwnerController);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Tasks")
        void OnTaskCompleted();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Tasks")
        void OnTaskTick();

    UFUNCTION(BlueprintCallable, Category = "Tasks")
        bool CompleteTask();

    //UObject
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

    // FTickableGameObject Begin
    virtual void Tick(float DeltaTime) override;
    virtual ETickableTickType GetTickableTickType() const override
    {
        return ETickableTickType::Always;
    }
    virtual TStatId GetStatId() const override
    {
        RETURN_QUICK_DECLARE_CYCLE_STAT(FMyTickableThing, STATGROUP_Tickables);
    }
    

#if WITH_EDITOR
    virtual FText GetNodeTitle() const override;

    virtual void SetNodeTitle(const FText& NewTitle) override;

    virtual FLinearColor GetBackgroundColor() const override;
#endif
private:
    // The last frame number we were ticked.
    // We don't want to tick multiple times per frame 
    uint32 LastFrameNumberWeTicked = INDEX_NONE;
};
