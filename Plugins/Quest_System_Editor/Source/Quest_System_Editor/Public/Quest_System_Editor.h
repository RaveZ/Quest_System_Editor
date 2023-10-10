// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include <EdGraphUtilities.h>
#include "QuestDetailPanel.h"

class FToolBarBuilder;
class FMenuBuilder;


class FQuest_System_EditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);


	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<FGraphPanelNodeFactory> GraphPanelNodeFactory_QuestEditor;
	TSharedPtr<class IDetailsView> PropertyWidget;
	UQuestDetailPanel* CustomSettings;
};
