// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraphNode.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystemGraphNode_Root.generated.h"

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	E_Inactive		UMETA(DisplayName = "INACTIVE"),
	E_Active		UMETA(DisplayName = "ACTIVE"),
	E_Complete		UMETA(DisplayName = "COMPLETE"),
	E_Fail			UMETA(DisplayName = "FAIL"),
};

UCLASS()
class QUEST_SYSTEM_RUNTIME_API UQuestSystemGraphNode_Root : public UQuestSystemGraphNode
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraphNode_Root")
	EQuestState QuestState = EQuestState::E_Active;
};
