// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FAssetEditor_QuestSystem;
class FExtender;
class FToolBarBuilder;

class QUEST_SYSTEM_EDITOR_API FAssetEditorToolbar_QuestSystem : public TSharedFromThis<FAssetEditorToolbar_QuestSystem>
{
public:
	FAssetEditorToolbar_QuestSystem(TSharedPtr<FAssetEditor_QuestSystem> InGenericGraphEditor)
		: QuestSystemEditor(InGenericGraphEditor) {}

	void AddQuestSystemToolbar(TSharedPtr<FExtender> Extender);

private:
	void FillQuestSystemToolbar(FToolBarBuilder& ToolbarBuilder);

protected:
	/** Pointer back to the blueprint editor tool that owns us */
	TWeakPtr<FAssetEditor_QuestSystem> QuestSystemEditor;
};
