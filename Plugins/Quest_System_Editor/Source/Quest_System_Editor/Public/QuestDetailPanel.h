// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestDetailPanel.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, config = EditorPerProjectUserSettings)
class QUEST_SYSTEM_EDITOR_API UQuestDetailPanel : public UObject
{
	GENERATED_BODY()
    UPROPERTY(EditAnywhere, category = "Custom Settings")
        UStaticMesh* CustomMesh;

    UPROPERTY(EditAnywhere, category = "Custom Settings")
        float CustomFloat;

    UPROPERTY(EditAnywhere, category = "Other Settings")
        bool CustomBool;
	
};
