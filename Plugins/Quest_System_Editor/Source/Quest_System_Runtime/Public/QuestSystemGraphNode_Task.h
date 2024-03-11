// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraphNode.h"
#include "Tickable.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystemGraphNode_Task.generated.h"

class UQuestSystemGraphNode_TaskManager;



UENUM(BlueprintType)
enum class ETaskState : uint8
{
    E_Active		UMETA(DisplayName = "ACTIVE"),
    E_Complete		UMETA(DisplayName = "COMPLETE"),
};

UCLASS(Blueprintable, EditInlineNew, HideDropdown)
class QUEST_SYSTEM_RUNTIME_API UQuestSystemGraphNode_Task : public UQuestSystemGraphNode, public FTickableGameObject
{
	GENERATED_BODY()
public:
    UQuestSystemGraphNode_Task();

    UPROPERTY(BlueprintReadWrite)
        int CurrentAmount;


    UPROPERTY(BlueprintReadWrite, Category = "Tasks")
        ETaskState TaskState = ETaskState::E_Active;

    UPROPERTY(BlueprintReadOnly, Category = "Tasks")
        UQuestSystemGraphNode_TaskManager* TaskManager;

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

    virtual FText GetNodeDescription() const override;

    virtual FLinearColor GetBackgroundColor() const override;
#endif
private:
    // The last frame number we were ticked.
    // We don't want to tick multiple times per frame 
    uint32 LastFrameNumberWeTicked = INDEX_NONE;
};
