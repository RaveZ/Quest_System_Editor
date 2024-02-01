// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetEditorToolbar_QuestSystem.h"
#include "Quest_System_EditorCommands.h"
#include "Quest_System_EditorStyle.h"
#include "AssetEditor_QuestSystem.h"

#define LOCTEXT_NAMESPACE "AssetEditorToolbar_QuestSystem"
void FAssetEditorToolbar_QuestSystem::AddQuestSystemToolbar(TSharedPtr<FExtender> Extender)
{
	check(QuestSystemEditor.IsValid());
	TSharedPtr<FAssetEditor_QuestSystem> QuestSystemEditorPtr = QuestSystemEditor.Pin();
	
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, QuestSystemEditorPtr->GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP(this, &FAssetEditorToolbar_QuestSystem::FillQuestSystemToolbar));
	QuestSystemEditorPtr->AddToolbarExtender(ToolbarExtender);
}

void FAssetEditorToolbar_QuestSystem::FillQuestSystemToolbar(FToolBarBuilder& ToolbarBuilder)
{
	check(QuestSystemEditor.IsValid());
	TSharedPtr<FAssetEditor_QuestSystem> QuestSystemEditorPtr = QuestSystemEditor.Pin();

	ToolbarBuilder.BeginSection("Quest System");
	{
		const FText NewTaskLabel = LOCTEXT("NewTask_Label", "New Task");
		const FText NewTaskTooltip = LOCTEXT("NewTask_ToolTip", "Create a new task node Blueprint from a base class");
		const FSlateIcon NewTaskIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "BTEditor.Graph.NewTask");

		
		ToolbarBuilder.AddToolBarButton(FQuest_System_EditorCommands::Get().NewTask,
			NAME_None,
			NewTaskLabel,
			NewTaskTooltip,
			NewTaskIcon
		);
	}
	ToolbarBuilder.EndSection();

	ToolbarBuilder.BeginSection("Util");
	{
		ToolbarBuilder.AddToolBarButton(FQuest_System_EditorCommands::Get().AutoArrange,
			NAME_None,
			LOCTEXT("AutoArrange_Label", "Auto Arrange"),
			LOCTEXT("AutoArrange_ToolTip", "Auto arrange nodes, not perfect, but still handy"),
			FSlateIcon(FQuest_System_EditorStyle::GetStyleSetName(), "QuestSystemEditor.AutoArrange"));
	}
	ToolbarBuilder.EndSection();
}



#undef LOCTEXT_NAMESPACE
