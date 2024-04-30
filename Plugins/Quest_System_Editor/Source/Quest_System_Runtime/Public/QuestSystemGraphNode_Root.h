// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraphNode.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystemGraphNode_Root.generated.h"



UCLASS(HideCategories = ("Events", "Conditions"))
class QUEST_SYSTEM_RUNTIME_API UQuestSystemGraphNode_Root : public UQuestSystemGraphNode
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	virtual FText GetNodeDescription() const override;
#endif
	
};
