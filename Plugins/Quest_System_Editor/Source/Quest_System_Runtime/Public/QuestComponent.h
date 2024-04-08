// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.generated.h"

class UQuestSaveObject;
class USaveGame;

//Delegate
/** This Delegate will be called if a quest objective has been completed*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestObjectiveUpdated, UQuestSystem*, UpdatedQuest);

/** This Delegate will be called if a quest is unlocked or has failed. Used for notification widget*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestStateUpdated, UQuestSystem*, UpdatedQuest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, HideDropdown, ClassGroup = "Quest")
class QUEST_SYSTEM_RUNTIME_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

	UPROPERTY(BlueprintReadOnly, Category = "QuestComponent")
		UQuestComponent* QuestComponent;

	UPROPERTY(BlueprintReadOnly, Category = "QuestComponent")
		AController* OwningController;

	UPROPERTY(BlueprintReadOnly, Category = "QuestComponent")
		APawn* OwningPawn;

	/** Array of QuestGraph that is Running*/
	UPROPERTY(BlueprintReadWrite, Category = "QuestComponent")
		TArray<UQuestSystemGraph*> QuestGraphs;

	/** Map of Quest ID and attached Quest System That is Running*/
	UPROPERTY(BlueprintReadWrite, Category = "QuestComponent")
		TMap<FName, UQuestSystem*> QuestSystemMap;

	UFUNCTION(BlueprintCallable, Category = "QuestComponent")
		bool RunQuest(UQuestSystemGraph* QuestAsset);

	UFUNCTION(BlueprintCallable, Category = "QuestComponent")
		void RestartQuest(FName QuestID, FName NodeID = NAME_None, bool bStartFromNodeID = false);
	
	UFUNCTION(BlueprintCallable, Category = "QuestComponent")
		void FailQuest(FName QuestID);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "QuestComponent")
		FQuestObjectiveUpdated OnQuestObjectiveUpdated;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "QuestComponent")
		FQuestStateUpdated OnQuestStateUpdated;

	UFUNCTION(BlueprintCallable, Category = "QuestComponent")
		void OnUpdateQuest(UQuestSystem* UpdatedQuest);

	UFUNCTION(BlueprintCallable, Category = "QuestComponent")
		void EvaluateNextNodeCondition(UQuestSystem* QuestParam);

	UFUNCTION(BlueprintCallable, Category = "QuestComponent")
		void GoToNextNode(UQuestSystem* QuestParam, UQuestSystemGraphNode* NodeParam);

	UFUNCTION(BlueprintCallable, Category = "QuestComponent")
		void BeginNode(UQuestSystem* QuestParam);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Save(const FString& SaveFileName, int SlotIndex) const;

	virtual void Save_Implementation(const FString& SaveFileName, int SlotIndex) const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Load(const FString& SaveFileName, int SlotIndex) const;

	virtual void Load_Implementation(const FString& SaveFileName, int SlotIndex) const;

	

	void LoadGameDelegateFunction(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
