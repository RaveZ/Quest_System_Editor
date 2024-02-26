// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraph.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_QuestSystem.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_SYSTEM_RUNTIME_API UBFL_QuestSystem : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static UQuestSystemGraph* RunQuest(UQuestSystemGraph* QuestAsset);
};
