// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Setting_QuestEditor.h"
#include "QuestSystemGraph.h"

namespace QuestSectionID
{
	enum Type
	{
		NONE = 0,
		QUESTGRAPH,					// Quest Graph
		//Add More to populate the section
	};
};

class QUEST_SYSTEM_EDITOR_API FAssetEditor_QuestSystem : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
public:
	FAssetEditor_QuestSystem();
	virtual	~FAssetEditor_QuestSystem();

	void InitQuestSystemGraphAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UQuestSystemGraph* Graph);

	// IToolkit interface
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	// End of IToolkit interface

	// FAssetEditorToolkit
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FString GetDocumentationLink() const override;
	virtual void SaveAsset_Execute() override;
	// End of FAssetEditorToolkit
	
	//Toolbar
	void UpdateToolbar();
	TSharedPtr<class FAssetEditorToolbar_QuestSystem> GetToolbarBuilder() { return ToolbarBuilder; }
	void RegisterToolbarTab(const TSharedRef<class FTabManager>& TabManager);

	
	// FSerializableObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	// End of FSerializableObject interface

	/** Returns a pointer to the Quest graph object we are currently editing, as long as we are editing exactly one */
	virtual UQuestSystemGraph* GetQuestSystemObj() const;
protected:
	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_EditorSettings(const FSpawnTabArgs& Args); 
	TSharedRef<SDockTab> SpawnTab_MyQuest(const FSpawnTabArgs& Args);
	
	void CreateInternalWidgets();
	TSharedRef<SGraphEditor> CreateViewportWidget();

	
	void BindCommands();

	void CreateEdGraph();

	void CreateCommandList();
	
	
	TSharedPtr<SGraphEditor> GetCurrGraphEditor() const;

	FGraphPanelSelectionSet GetSelectedNodes() const;

	void RebuildQuestSystemGraph();

	// Delegates for graph editor commands
	void SelectAllNodes();
	bool CanSelectAllNodes();
	void DeleteSelectedNodes();
	bool CanDeleteNodes();
	void DeleteSelectedDuplicatableNodes();
	void CutSelectedNodes();
	bool CanCutNodes();
	void CopySelectedNodes();
	bool CanCopyNodes();
	void PasteNodes();
	void PasteNodesHere(const FVector2D& Location);
	bool CanPasteNodes();
	void DuplicateNodes();
	bool CanDuplicateNodes();

	//New Task Functions from toolbar
	void CreateNewTask();
	void HandleNewNodeClassPicked(UClass* InClass) const;
	bool CanCreateNewTask() const;
	bool IsNewTaskButtonVisible() const;

	void AutoArrange();
	bool CanAutoArrange() const;
	
	void OnRenameNode();
	bool CanRenameNodes() const;

	//////////////////////////////////////////////////////////////////////////
	// graph editor event
	void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);

	void OnNodeDoubleClicked(UEdGraphNode* Node);
	
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);

#if ENGINE_MAJOR_VERSION < 5
	void OnPackageSaved(const FString& PackageFileName, UObject* Outer);
#else // #if ENGINE_MAJOR_VERSION < 5
	void OnPackageSavedWithContext(const FString& PackageFileName, UPackage* Package, FObjectPostSaveContext ObjectSaveContext);
#endif // #else // #if ENGINE_MAJOR_VERSION < 5
protected:
	UQuestEditorSettings* QuestGraphEditorSettings;

	UQuestSystemGraph* EditingGraph;

	//Toolbar
	TSharedPtr<class FAssetEditorToolbar_QuestSystem> ToolbarBuilder;

	/** Handle to the registered OnPackageSave delegate */
	FDelegateHandle OnPackageSavedDelegateHandle;

	TSharedPtr<SGraphEditor> ViewportWidget;
	TSharedPtr<class IDetailsView> PropertyWidget;
	TSharedPtr<class IDetailsView> EditorSettingsWidget;
	TSharedPtr<class SMyQuest> MyQuestWidget;

	/** The command list for this editor */
	TSharedPtr<FUICommandList> GraphEditorCommands;


};
