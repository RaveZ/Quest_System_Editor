// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class QUEST_SYSTEM_EDITOR_API SGraphNode_QuestSystemEdge : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGraphNode_QuestSystemEdge)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
