// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystem.generated.h"

class UQuestCondition;
class UQuestSystemGraphNode;
class UQuestSystemGraphEdge;
class UQuestSystemGraphNode_Root;

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	/**Quest is Active*/
	E_Active		UMETA(DisplayName = "ACTIVE"),
	/**Quest is Completed*/
	E_Complete		UMETA(DisplayName = "COMPLETE"),
	/**Quest is Fail*/
	E_Fail			UMETA(DisplayName = "FAIL"),
	/**Quest is Locked, Waiting to be Unlocked*/
	E_Locked		UMETA(DisplayName = "LOCKED"),
	/**Quest is Unlocked, Waiting to be Activated*/
	E_Unlocked		UMETA(DisplayName = "UNLOCKED"),
};

UCLASS(Blueprintable)
class QUEST_SYSTEM_RUNTIME_API UQuestSystem : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleDefaultsOnly, Category = "QuestAssets")
		UQuestSystemGraph* QuestGraph;

	/** Quest ID */
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "QuestSystem")
		FName QuestID;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "QuestSystem")
		FText Description; 


	/**
	* TRUE : After the conditions met, activate this quest. if there are no conditions, activate this quest immediately
	* FALSE : After the conditions met, unlock this quest.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Conditions")
		bool bAutoActivateQuest;

	/**Conditions to unlock the Quest*/
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = "Conditions")
		TArray<UQuestCondition*> Conditions;

	/** List of All Visited Node IDs in Sequence*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestSystem")
		TArray<FName> VisitedNodeIDs;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestSystem")
		EQuestState QuestState = EQuestState::E_Locked;
	
	UPROPERTY(BlueprintReadWrite, Category = "QuestSystem")
		UQuestSystemGraphNode* CurrentNode;

	UPROPERTY(BlueprintReadOnly, Category = "QuestSystem")
		TArray<UQuestSystemGraphNode*> RootNodes;

	UPROPERTY(BlueprintReadOnly, Category = "QuestSystem")
		TArray<UQuestSystemGraphNode*> AllNodes;

	UPROPERTY(BlueprintReadOnly, Category = "QuestSystem")
		TMap<FName, UQuestSystemGraphNode*> NodeMap;


	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		void Print(bool ToConsole = true, bool ToScreen = true);


	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		int GetLevelNum() const;

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		void GetNodesByLevel(int Level, TArray<UQuestSystemGraphNode*>& Nodes);

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		AController* GetOwningController();

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		APawn* GetOwningPawn();

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		UQuestComponent* GetQuestComponent();

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		UQuestSystemGraph* GetOwningQuestGraph() const;
	void ClearGraph();


	

	

};

