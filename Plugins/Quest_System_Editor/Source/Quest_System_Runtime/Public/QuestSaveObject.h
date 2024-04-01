// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "QuestTask.h"
#include "QuestSaveObject.generated.h"

/** Task Data To Be Saved */
USTRUCT(BlueprintType)
struct FTaskData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "CurrentAmount", MakeStructureDefaultValue = "0"))
	int CurrentAmount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETaskState TaskState;
};

/** Node Data To Be Saved */
USTRUCT(BlueprintType)
struct FNodeData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "NodeID", MakeStructureDefaultValue = "None"))
	FName NodeID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "TaskDatas"))
	TArray<FTaskData> TaskDatas;
};

/** Quest Data To Be Saved */
USTRUCT(BlueprintType)
struct FQuestData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "QuestID", MakeStructureDefaultValue = "None"))
	FName QuestID;
	
	/** Map of Node ID and it's Node Data Struct */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "NodeMapData", MakeStructureDefaultValue = "()"))
	TMap<FName, FNodeData> NodeMapData;
};

UCLASS(Blueprintable)
class QUEST_SYSTEM_RUNTIME_API UQuestSaveObject : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FQuestData> QuestDatas;
};
