// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <EdGraphUtilities.h>
#include <EdGraph/EdGraphNode.h>
#include "CoreMinimal.h"

/**
 * 
 */
class FQuestSystemNodeFactory : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override;
	
};
