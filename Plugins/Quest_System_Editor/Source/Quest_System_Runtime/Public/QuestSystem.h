// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraphNode.h"
#include "QuestSystemGraphNode_Root.h"
#include "QuestSystemGraphEdge.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystem.generated.h"


UENUM(BlueprintType)
enum class EQuestState : uint8
{
	E_Inactive		UMETA(DisplayName = "INACTIVE"),
	E_Active		UMETA(DisplayName = "ACTIVE"),
	E_Complete		UMETA(DisplayName = "COMPLETE"),
	E_Fail			UMETA(DisplayName = "FAIL"),
};

UCLASS(Blueprintable)
class QUEST_SYSTEM_RUNTIME_API UQuestSystem : public UObject
{
	GENERATED_BODY()
public:
	/** Quest ID */
	UPROPERTY(Category = Description, EditAnywhere)
		FString QuestID;
		
	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraphNode_Root")
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
	int GetLevelNum() const;

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void GetNodesByLevel(int Level, TArray<UQuestSystemGraphNode*>& Nodes);

	void ClearGraph();
};
