// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.generated.h"


//Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestObjectiveUpdated, UQuestSystem*, UpdatedQuest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, HideDropdown)
class QUEST_SYSTEM_RUNTIME_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

	UPROPERTY(BlueprintReadWrite, Category = "QuestComponent")
		UQuestComponent* QuestComponent;

	UPROPERTY(BlueprintReadOnly, Category = "QuestComponent")
		AController* OwningController;

	UPROPERTY(BlueprintReadWrite, Category = "QuestComponent")
		APawn* OwningPawn;

	UPROPERTY(BlueprintReadOnly, Category = "QuestComponent")
		TArray<UQuestSystemGraph*> QuestGraphs;

	UFUNCTION(BlueprintCallable, Category = "QuestComponent")
		bool StartQuest(UQuestSystemGraph* QuestAsset);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "QuestComponent")
		FQuestObjectiveUpdated OnQuestObjectiveUpdated;

	UFUNCTION(BlueprintCallable, Category = "QuestComponent")
		void OnUpdateQuest(UQuestSystem* UpdatedQuest);

	UFUNCTION(BlueprintCallable, Category = "QuestComponent")
		void EvaluateNextNodeCondition(UQuestSystem* QuestParam);

	UFUNCTION(BlueprintCallable, Category = "QuestComponent")
		void GoToNextNode(UQuestSystem* QuestParam, UQuestSystemGraphNode* NodeParam);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
