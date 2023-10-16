// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetTypeActions_QuestEditor.h"
#include "QuestSystemGraph.h"
#include "AssetEditor_QuestSystem.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions_QuestEditor"
FAssetTypeActions_QuestEditor::FAssetTypeActions_QuestEditor(EAssetTypeCategories::Type InAssetCategory)
    : MyAssetCategory(InAssetCategory)
{
}

FText FAssetTypeActions_QuestEditor::GetName() const
{
    return LOCTEXT("FQuestSystemAssetTypeActionsName", "QuestSystemGraph");
}

FColor FAssetTypeActions_QuestEditor::GetTypeColor() const
{
    return FColor(129, 196, 115);
}

UClass* FAssetTypeActions_QuestEditor::GetSupportedClass() const
{
    return UQuestSystemGraph::StaticClass();
}

void FAssetTypeActions_QuestEditor::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UQuestSystemGraph* Graph = Cast<UQuestSystemGraph>(*ObjIt))
		{
			TSharedRef<FAssetEditor_QuestSystem> NewGraphEditor(new FAssetEditor_QuestSystem());
			NewGraphEditor->InitQuestSystemGraphAssetEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}

uint32 FAssetTypeActions_QuestEditor::GetCategories()
{
    return MyAssetCategory;
}

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE