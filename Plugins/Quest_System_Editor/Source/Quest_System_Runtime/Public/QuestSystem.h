// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystem.generated.h"

class UQuestSystemGraphNode;
class UQuestSystemGraphEdge;
class UQuestSystemGraphNode_Root;
class UQuestSystemGraphNode_Task;

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	E_Active		UMETA(DisplayName = "ACTIVE"),
	E_Complete		UMETA(DisplayName = "COMPLETE"),
	E_Fail			UMETA(DisplayName = "FAIL"),
	E_Inactive		UMETA(DisplayName = "INACTIVE"),
};

UCLASS(Blueprintable)
class QUEST_SYSTEM_RUNTIME_API UQuestSystem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleDefaultsOnly, Category = "QuestAssets")
		UQuestSystemGraph* QuestGraph;

	/** Quest ID */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "QuestSystem")
		FName QuestID;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "QuestSystem")
		FText Description;

		
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestSystem")
		EQuestState QuestState = EQuestState::E_Active;
	
	UPROPERTY(BlueprintReadOnly, Category = "QuestSystem")
		UQuestSystemGraphNode* CurrentNode;

	UPROPERTY(BlueprintReadOnly, Category = "QuestSystem")
		TArray<UQuestSystemGraphNode*> RootNodes;

	UPROPERTY(BlueprintReadOnly, Category = "QuestSystem")
		TArray<UQuestSystemGraphNode*> AllNodes;

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		void Print(bool ToConsole = true, bool ToScreen = true);

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		void GoToNextNode();

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
		UQuestSystemGraph* GetOwningQuest() const;
	void ClearGraph();
};
