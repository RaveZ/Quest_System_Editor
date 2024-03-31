// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystemGraphNode_Task.generated.h"

class UQuestSystemGraphNode_TaskManager;
class UQuestSystemGraph;
class UQuestSystem;




UCLASS(Blueprintable, EditInlineNew, HideDropdown, HideCategories = ("Events", "Conditions"))
class QUEST_SYSTEM_RUNTIME_API UQuestSystemGraphNode_Task : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
    UQuestSystemGraphNode_Task();

    UPROPERTY(BlueprintReadOnly, Category = "Task")
        UQuestSystemGraph* QuestGraph;

    UPROPERTY(BlueprintReadOnly, Category = "Task")
        UQuestSystem* QuestSystem;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detail")
        FText Description;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Task")
        int CurrentAmount;

    UPROPERTY(BlueprintReadWrite, Category = "Task")
        float DistanceToTarget;



    UPROPERTY(BlueprintReadOnly, Category = "Tasks")
        UQuestSystemGraphNode_TaskManager* TaskManager;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Task")
        void OnTaskBegin();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Task")
        void OnTaskCompleted();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Task")
        void OnTaskTick();

    UFUNCTION(BlueprintCallable, Category = "Task")
        bool CompleteTask();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Task")
    FText GetDescription() const;


    virtual FText GetDescription_Implementation() const;

    UFUNCTION(BlueprintCallable, Category = "Task")
        AController* GetOwningController() const;

    UFUNCTION(BlueprintCallable, Category = "Task")
        APawn* GetOwningPawn() const;

    UFUNCTION(BlueprintCallable, Category = "Task")
        UQuestComponent* GetQuestComponent() const;

    UFUNCTION(BlueprintCallable, Category = "Task")
        UQuestSystemGraph* GetOwningQuestGraph() const;

    UFUNCTION(BlueprintCallable, Category = "Task")
        UQuestSystem* GetOwningQuestSystem() const;
   
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
    


private:
    // The last frame number we were ticked.
    // We don't want to tick multiple times per frame 
    uint32 LastFrameNumberWeTicked = INDEX_NONE;
};
