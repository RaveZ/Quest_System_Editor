// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "UObject/NoExportTypes.h"
#include "QuestTask.generated.h"


class UQuestSystemGraph;
class UQuestSystem;
class UQuestComponent;
class UQuestSystemGraphNode_Objective;




UENUM(BlueprintType)
enum class ETaskState : uint8
{
	E_Active		UMETA(DisplayName = "ACTIVE"),
	E_Complete		UMETA(DisplayName = "COMPLETE"),
};
UCLASS(Blueprintable, EditInlineNew, HideDropdown, HideCategories = ("Hidden"))
class QUEST_SYSTEM_RUNTIME_API UQuestTask : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Task")
		FText Description;

	//mark the task as optional and skip this task if the other non-optional task has been completed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		bool bOptional = false;
	
	/**This task use amount required to complete task?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (DisplayName = "bUseAmount?"))
		bool bUseAmount = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (EditCondition = "bUseAmount == true", EditConditionHides))
		int RequiredAmount = 1;

	UPROPERTY(BlueprintReadOnly, Category = "QuestTask")
		UQuestSystemGraph* QuestGraph;

	UPROPERTY(BlueprintReadOnly, Category = "QuestTask")
		UQuestSystem* QuestSystem;


	UPROPERTY(BlueprintReadOnly, Category = "QuestTask")
		UQuestSystemGraphNode_Objective* ObjectiveNode;

	UPROPERTY(BlueprintReadWrite, Category = "QuestTask")
		ETaskState TaskState = ETaskState::E_Active;

	UPROPERTY(BlueprintReadWrite, Category = "QuestTask")
		int CurrentAmount = 0;

	UPROPERTY(BlueprintReadWrite, Category = "QuestTask")
		float DistanceToTarget;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "QuestTask")
		void TaskBegin();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "QuestTask")
		void TaskTick(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "QuestTask")
		void OnTaskCompleted();

	UFUNCTION(BlueprintCallable, Category = "QuestTask")
		bool CompleteTask();

	UFUNCTION(BlueprintCallable, Category = "QuestTask")
		AController* GetOwningController() const;

	UFUNCTION(BlueprintCallable, Category = "QuestTask")
		UQuestComponent* GetQuestComponent() const;

	UFUNCTION(BlueprintCallable, Category = "QuestTask")
		UQuestSystemGraph* GetOwningQuestGraph() const;

	UFUNCTION(BlueprintCallable, Category = "QuestTask")
		UQuestSystem* GetOwningQuestSystem() const;

	/**Add amount and check if the required amount has fulfilled, then complete the task*/
	UFUNCTION(BlueprintCallable, Category = "QuestTask")
		void AddAmount(int AmountToAdd);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuestTask")
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
