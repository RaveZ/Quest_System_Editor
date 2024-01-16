// Copyright Epic Games, Inc. All Rights Reserved.

#include "PluginTutsEditor.h"

#define LOCTEXT_NAMESPACE "FPluginTutsEditor"

void FPluginTutsEditor::StartupModule()
{
	NormalDistributionAssetTypeActions = MakeShared<FNormalDistributionAssetTypeActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(NormalDistributionAssetTypeActions.ToSharedRef());

}

void FPluginTutsEditor::ShutdownModule()
{
	if (!FModuleManager::Get().IsModuleLoaded("AssetTools")) return;
	FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(NormalDistributionAssetTypeActions.ToSharedRef());
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPluginTutsEditor, PluginTutsEditor)