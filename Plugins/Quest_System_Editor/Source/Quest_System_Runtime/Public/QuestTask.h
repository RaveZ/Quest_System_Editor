// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestTask.generated.h"


class UQuestSystemGraph;
class UQuestSystem;
class UQuestComponent;
class UQuestSystemGraphNode_TaskManager;


UENUM(BlueprintType)
enum class ETaskState : uint8
{
	E_Active		UMETA(DisplayName = "ACTIVE"),
	E_Complete		UMETA(DisplayName = "COMPLETE"),
};
UCLASS(Blueprintable, EditInlineNew, HideDropdown)
class QUEST_SYSTEM_RUNTIME_API UQuestTask : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detail")
		FText Description;

	UPROPERTY(BlueprintReadOnly, Category = "QuestTask")
		UQuestSystemGraph* QuestGraph;

	UPROPERTY(BlueprintReadOnly, Category = "QuestTask")
		UQuestSystem* QuestSystem;


	UPROPERTY(BlueprintReadOnly, Category = "Tasks")
		UQuestSystemGraphNode_TaskManager* TaskManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Task")
		ETaskState TaskState = ETaskState::E_Active;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Task")
		int CurrentAmount;

	UPROPERTY(BlueprintReadWrite, Category = "Task")
		float DistanceToTarget;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "QuestTask")
		void TaskBegin();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Task")
		void TaskTick();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Task")
		void OnTaskCompleted();

	UFUNCTION(BlueprintCallable, Category = "Task")
		bool CompleteTask();

	UFUNCTION(BlueprintCallable, Category = "QuestTask")
		AController* GetOwningController() const;

	UFUNCTION(BlueprintCallable, Category = "QuestTask")
		APawn* GetOwningPawn() const;

	UFUNCTION(BlueprintCallable, Category = "QuestTask")
		UQuestComponent* GetQuestComponent() const;

	UFUNCTION(BlueprintCallable, Category = "QuestTask")
		UQuestSystemGraph* GetOwningQuestGraph() const;

	UFUNCTION(BlueprintCallable, Category = "QuestTask")
		UQuestSystem* GetOwningQuestSystem() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Task")
		FText GetDescription() const;


	virtual FText GetDescription_Implementation() const;

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
