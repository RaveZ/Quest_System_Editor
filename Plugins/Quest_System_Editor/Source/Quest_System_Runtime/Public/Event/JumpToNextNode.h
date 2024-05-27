// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Event/QuestEvent.h"
#include "JumpToNextNode.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_SYSTEM_RUNTIME_API UJumpToNextNode : public UQuestEvent
{
	GENERATED_BODY()
public:
	UJumpToNextNode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestEvent", meta = (GetOptions = "GetNextNodeID") )
		FName NodeID;

	UFUNCTION(CallInEditor)
		TArray<FName> GetNextNodeID();

	virtual void BeginEvent_Implementation() override;
};
