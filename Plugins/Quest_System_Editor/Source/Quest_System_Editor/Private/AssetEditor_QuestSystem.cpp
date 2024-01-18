// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetEditor_QuestSystem.h"
#include "UObject/ObjectSaveContext.h"
#include "GraphEditorActions.h"
#include "Framework/Commands/GenericCommands.h"
#include "AssetEditorToolbar_QuestSystem.h"
#include "Quest_System_EditorCommands.h"
#include "EdGraph_QuestSystemGraph.h"
#include "EdNode_QuestSystemNode.h"
#include "EdNode_QuestSystemEdge.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "HAL/PlatformApplicationMisc.h"
#include "AssetGraphSchema_QuestSystem.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include <Quest_System_Editor.h>
#include <ContentBrowserFrontEndFilterExtension.h>
#include <ContentBrowserModule.h>
#include <Kismet2/KismetEditorUtilities.h>

#define LOCTEXT_NAMESPACE "AssetEditor_GenericGraph"

struct FQuestSystemAssetEditorTabs
{
	// Tab identifiers
	static const FName GenericGraphPropertyID;
	static const FName ViewportID;
	static const FName GenericGraphEditorSettingsID;
};

//////////////////////////////////////////////////////////////////////////

const FName FQuestSystemAssetEditorTabs::GenericGraphPropertyID(TEXT("GenericGraphProperty"));
const FName FQuestSystemAssetEditorTabs::ViewportID(TEXT("Viewport"));
const FName FQuestSystemAssetEditorTabs::GenericGraphEditorSettingsID(TEXT("GenericGraphEditorSettings"));

//////////////////////////////////////////////////////////////////////////

const FName GenericGraphEditorAppName = FName(TEXT("GenericGraphEditorApp"));

FAssetEditor_QuestSystem::FAssetEditor_QuestSystem()
{
	EditingGraph = nullptr;

	QuestGraphEditorSettings = NewObject<UQuestEditorSettings>(UQuestEditorSettings::StaticClass());

#if ENGINE_MAJOR_VERSION < 5
	OnPackageSavedDelegateHandle = UPackage::PackageSavedEvent.AddRaw(this, &FAssetEditor_QuestSystem::OnPackageSaved);
#else // #if ENGINE_MAJOR_VERSION < 5
	OnPackageSavedDelegateHandle = UPackage::PackageSavedWithContextEvent.AddRaw(this, &FAssetEditor_QuestSystem::OnPackageSavedWithContext);
#endif // #else // #if ENGINE_MAJOR_VERSION < 5
}

FAssetEditor_QuestSystem::~FAssetEditor_QuestSystem()
{
#if ENGINE_MAJOR_VERSION < 5
	UPackage::PackageSavedEvent.Remove(OnPackageSavedDelegateHandle);
#else // #if ENGINE_MAJOR_VERSION < 5
	UPackage::PackageSavedWithContextEvent.Remove(OnPackageSavedDelegateHandle);
#endif // #else // #if ENGINE_MAJOR_VERSION < 5
}

void FAssetEditor_QuestSystem::InitQuestSystemGraphAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UQuestSystemGraph* Graph)
{
	EditingGraph = Graph;
	CreateEdGraph();
	FGenericCommands::Register();
	FGraphEditorCommands::Register();
	FQuest_System_EditorCommands::Register();

	if (!ToolbarBuilder.IsValid())
	{
		ToolbarBuilder = MakeShareable(new FAssetEditorToolbar_QuestSystem(SharedThis(this)));
	}

	BindCommands();

	CreateInternalWidgets();

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	ToolbarBuilder->AddQuestSystemToolbar(ToolbarExtender);
	
	// Layout
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_GenericGraphEditor_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
#if ENGINE_MAJOR_VERSION < 5
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)->SetHideTabWell(true)
			)
#endif // #if ENGINE_MAJOR_VERSION < 5
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(0.9f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.65f)
					->AddTab(FQuestSystemAssetEditorTabs::ViewportID, ETabState::OpenedTab)->SetHideTabWell(true)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.7f)
						->AddTab(FQuestSystemAssetEditorTabs::GenericGraphPropertyID, ETabState::OpenedTab)->SetHideTabWell(true)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.3f)
						->AddTab(FQuestSystemAssetEditorTabs::GenericGraphEditorSettingsID, ETabState::OpenedTab)
					)
				)
			)
		);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, GenericGraphEditorAppName, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, EditingGraph, false);

	RegenerateMenusAndToolbars();
}

void FAssetEditor_QuestSystem::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_GenericGraphEditor", "Generic Graph Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FQuestSystemAssetEditorTabs::ViewportID, FOnSpawnTab::CreateSP(this, &FAssetEditor_QuestSystem::SpawnTab_Viewport))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(FQuestSystemAssetEditorTabs::GenericGraphPropertyID, FOnSpawnTab::CreateSP(this, &FAssetEditor_QuestSystem::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTab", "Property"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(FQuestSystemAssetEditorTabs::GenericGraphEditorSettingsID, FOnSpawnTab::CreateSP(this, &FAssetEditor_QuestSystem::SpawnTab_EditorSettings))
		.SetDisplayName(LOCTEXT("EditorSettingsTab", "Generic Graph Editor Setttings"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FAssetEditor_QuestSystem::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FQuestSystemAssetEditorTabs::ViewportID);
	InTabManager->UnregisterTabSpawner(FQuestSystemAssetEditorTabs::GenericGraphPropertyID);
	InTabManager->UnregisterTabSpawner(FQuestSystemAssetEditorTabs::GenericGraphEditorSettingsID);
}



void FAssetEditor_QuestSystem::CreateEdGraph()
{
	if (EditingGraph->EdGraph == nullptr)
	{
		EditingGraph->EdGraph = CastChecked<UEdGraph_QuestSystemGraph>(FBlueprintEditorUtils::CreateNewGraph(EditingGraph, NAME_None, UEdGraph_QuestSystemGraph::StaticClass(), UAssetGraphSchema_QuestSystem::StaticClass()));
		EditingGraph->EdGraph->bAllowDeletion = false;

		// Give the schema a chance to fill out any required nodes (like the results node)
		const UEdGraphSchema* Schema = EditingGraph->EdGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*EditingGraph->EdGraph);
	}
}

void FAssetEditor_QuestSystem::BindCommands()
{
	ToolkitCommands->MapAction(FQuest_System_EditorCommands::Get().NewTask,
		FExecuteAction::CreateSP(this, &FAssetEditor_QuestSystem::CreateNewTask),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_QuestSystem::CanCreateNewTask),
		FIsActionChecked(),
		FIsActionButtonVisible::CreateSP(this, &FAssetEditor_QuestSystem::IsNewTaskButtonVisible)
	);

	ToolkitCommands->MapAction(FQuest_System_EditorCommands::Get().AutoArrange,
		FExecuteAction::CreateSP(this, &FAssetEditor_QuestSystem::AutoArrange),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_QuestSystem::CanAutoArrange)
	);
}

void FAssetEditor_QuestSystem::CreateCommandList()
{
	if (GraphEditorCommands.IsValid()) {
		return;
	}

	GraphEditorCommands = MakeShareable(new FUICommandList);
	// Can't use CreateSP here because derived editor are already implementing TSharedFromThis<FAssetEditorToolkit>
	// however it should be safe, since commands are being used only within this editor
	// if it ever crashes, this function will have to go away and be reimplemented in each derived class

	GraphEditorCommands->MapAction(FQuest_System_EditorCommands::Get().NewTask,
		FExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::CreateNewTask),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::CanCreateNewTask));

	GraphEditorCommands->MapAction(FQuest_System_EditorCommands::Get().AutoArrange,
		FExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::AutoArrange),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::CanAutoArrange));

	GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
		FExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::SelectAllNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::CanSelectAllNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::CanDeleteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
		FExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::CanCopyNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
		FExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::CutSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::CanCutNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
		FExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::PasteNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::CanPasteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
		FExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::DuplicateNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_QuestSystem::CanDuplicateNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Rename,
		FExecuteAction::CreateSP(this, &FAssetEditor_QuestSystem::OnRenameNode),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_QuestSystem::CanRenameNodes)
	);
}

TSharedPtr<SGraphEditor> FAssetEditor_QuestSystem::GetCurrGraphEditor() const
{
	return ViewportWidget;
}

FGraphPanelSelectionSet FAssetEditor_QuestSystem::GetSelectedNodes() const
{
	FGraphPanelSelectionSet CurrentSelection;
	TSharedPtr<SGraphEditor> FocusedGraphEd = GetCurrGraphEditor();
	if (FocusedGraphEd.IsValid())
	{
		CurrentSelection = FocusedGraphEd->GetSelectedNodes();
	}

	return CurrentSelection;
}

FName FAssetEditor_QuestSystem::GetToolkitFName() const
{
	return FName("FQuestGraphEditor");
}

FText FAssetEditor_QuestSystem::GetBaseToolkitName() const
{
	return LOCTEXT("QuestGraphEditorAppLabel", "Quest Graph Editor");
}

FText FAssetEditor_QuestSystem::GetToolkitName() const
{
	const bool bDirtyState = EditingGraph->GetOutermost()->IsDirty();

	FFormatNamedArguments Args;
	Args.Add(TEXT("QuestGraphName"), FText::FromString(EditingGraph->GetName()));
	Args.Add(TEXT("DirtyState"), bDirtyState ? FText::FromString(TEXT("*")) : FText::GetEmpty());
	return FText::Format(LOCTEXT("QuestGraphEditorToolkitName", "{QuestGraphName}{DirtyState}"), Args);
}

FText FAssetEditor_QuestSystem::GetToolkitToolTipText() const
{
	return FAssetEditorToolkit::GetToolTipTextForObject(EditingGraph);
}

FLinearColor FAssetEditor_QuestSystem::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Blue;
}

FString FAssetEditor_QuestSystem::GetWorldCentricTabPrefix() const
{
	return TEXT("QuestGraphEditor");
}

FString FAssetEditor_QuestSystem::GetDocumentationLink() const
{
	//make documentation from notion add this link
	return TEXT("");
}

void FAssetEditor_QuestSystem::SaveAsset_Execute()
{
	if (EditingGraph != nullptr)
	{
		RebuildQuestSystemGraph();
	}

	FAssetEditorToolkit::SaveAsset_Execute();
}

void FAssetEditor_QuestSystem::UpdateToolbar()
{
}

void FAssetEditor_QuestSystem::RegisterToolbarTab(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void FAssetEditor_QuestSystem::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditingGraph);
	Collector.AddReferencedObject(EditingGraph->EdGraph);
}

TSharedRef<SDockTab> FAssetEditor_QuestSystem::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FQuestSystemAssetEditorTabs::ViewportID);

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Label(LOCTEXT("ViewportTab_Title", "Viewport"));

	if (ViewportWidget.IsValid())
	{
		SpawnedTab->SetContent(ViewportWidget.ToSharedRef());
	}

	return SpawnedTab;
}

TSharedRef<SDockTab> FAssetEditor_QuestSystem::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FQuestSystemAssetEditorTabs::GenericGraphPropertyID);

	return SNew(SDockTab)
#if ENGINE_MAJOR_VERSION < 5
		.Icon(FAppStyle::GetBrush("LevelEditor.Tabs.Details"))
#endif // #if ENGINE_MAJOR_VERSION < 5
		.Label(LOCTEXT("Details_Title", "Property"))
		[
			PropertyWidget.ToSharedRef()
		];
}

TSharedRef<SDockTab> FAssetEditor_QuestSystem::SpawnTab_EditorSettings(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FQuestSystemAssetEditorTabs::GenericGraphEditorSettingsID);

	return SNew(SDockTab)
#if ENGINE_MAJOR_VERSION < 5
		.Icon(FAppStyle::GetBrush("LevelEditor.Tabs.Details"))
#endif // #if ENGINE_MAJOR_VERSION < 5
		.Label(LOCTEXT("EditorSettings_Title", "Generic Graph Editor Setttings"))
		[
			EditorSettingsWidget.ToSharedRef()
		];
}

void FAssetEditor_QuestSystem::CreateInternalWidgets()
{
	ViewportWidget = CreateViewportWidget();

	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyWidget = PropertyModule.CreateDetailView(Args);
	PropertyWidget->SetObject(EditingGraph);
	PropertyWidget->OnFinishedChangingProperties().AddSP(this, &FAssetEditor_QuestSystem::OnFinishedChangingProperties);

	EditorSettingsWidget = PropertyModule.CreateDetailView(Args);
	EditorSettingsWidget->SetObject(QuestGraphEditorSettings);
}

TSharedRef<SGraphEditor> FAssetEditor_QuestSystem::CreateViewportWidget()
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_GenericGraph", "Quest System Graph");

	CreateCommandList();

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FAssetEditor_QuestSystem::OnSelectedNodesChanged);
	InEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &FAssetEditor_QuestSystem::OnNodeDoubleClicked);

	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(EditingGraph->EdGraph)
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);
}

void FAssetEditor_QuestSystem::RebuildQuestSystemGraph()
{
	if (EditingGraph == nullptr)
	{
		//LOG_WARNING(TEXT("FGenericGraphAssetEditor::RebuildGenericGraph EditingGraph is nullptr"));
		return;
	}

	UEdGraph_QuestSystemGraph* EdGraph = Cast<UEdGraph_QuestSystemGraph>(EditingGraph->EdGraph);
	check(EdGraph != nullptr);

	EdGraph->RebuildQuestSystemGraph();
}
void FAssetEditor_QuestSystem::SelectAllNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		CurrentGraphEditor->SelectAllNodes();
	}
}
bool FAssetEditor_QuestSystem::CanSelectAllNodes()
{
	return true;
}
void FAssetEditor_QuestSystem::DeleteSelectedNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());

	CurrentGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* EdNode = Cast<UEdGraphNode>(*NodeIt);
		if (EdNode == nullptr || !EdNode->CanUserDeleteNode())
			continue;;

		if (UEdNode_QuestSystemNode* EdNode_Node = Cast<UEdNode_QuestSystemNode>(EdNode))
		{
			EdNode_Node->Modify();

			const UEdGraphSchema* Schema = EdNode_Node->GetSchema();
			if (Schema != nullptr)
			{
				Schema->BreakNodeLinks(*EdNode_Node);
			}

			EdNode_Node->DestroyNode();
		}
		else
		{
			EdNode->Modify();
			EdNode->DestroyNode();
		}
	}
}
bool FAssetEditor_QuestSystem::CanDeleteNodes()
{
	// If any of the nodes can be deleted then we should allow deleting
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node != nullptr && Node->CanUserDeleteNode())
		{
			return true;
		}
	}
	return false;
}
void FAssetEditor_QuestSystem::DeleteSelectedDuplicatableNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FGraphPanelSelectionSet OldSelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}

	// Delete the duplicatable nodes
	DeleteSelectedNodes();

	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}
}
void FAssetEditor_QuestSystem::CutSelectedNodes()
{
	CopySelectedNodes();
	DeleteSelectedDuplicatableNodes();
}
bool FAssetEditor_QuestSystem::CanCutNodes()
{
	return CanCopyNodes() && CanDeleteNodes();
}

void FAssetEditor_QuestSystem::CopySelectedNodes()
{
	// Export the selected nodes and place the text on the clipboard
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	FString ExportedText;

	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node == nullptr)
		{
			SelectedIter.RemoveCurrent();
			continue;
		}

		if (UEdNode_QuestSystemEdge* EdNode_Edge = Cast<UEdNode_QuestSystemEdge>(*SelectedIter))
		{
			UEdNode_QuestSystemNode* StartNode = EdNode_Edge->GetStartNode();
			UEdNode_QuestSystemNode* EndNode = EdNode_Edge->GetEndNode();

			if (!SelectedNodes.Contains(StartNode) || !SelectedNodes.Contains(EndNode))
			{
				SelectedIter.RemoveCurrent();
				continue;
			}
		}

		Node->PrepareForCopying();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}
bool FAssetEditor_QuestSystem::CanCopyNodes()
{
	// If any of the nodes can be duplicated then we should allow copying
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			return true;
		}
	}

	return false;
}
void FAssetEditor_QuestSystem::PasteNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		PasteNodesHere(CurrentGraphEditor->GetPasteLocation());
	}
}
void FAssetEditor_QuestSystem::PasteNodesHere(const FVector2D& Location)
{
	// Find the graph editor with focus
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}
	// Select the newly pasted stuff
	UEdGraph* EdGraph = CurrentGraphEditor->GetCurrentGraph();

	{
		const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());
		EdGraph->Modify();

		// Clear the selection set (newly pasted stuff will be selected)
		CurrentGraphEditor->ClearSelectionSet();

		// Grab the text to paste from the clipboard.
		FString TextToImport;
		FPlatformApplicationMisc::ClipboardPaste(TextToImport);

		// Import the nodes
		TSet<UEdGraphNode*> PastedNodes;
		FEdGraphUtilities::ImportNodesFromText(EdGraph, TextToImport, PastedNodes);

		//Average position of nodes so we can move them while still maintaining relative distances to each other
		FVector2D AvgNodePosition(0.0f, 0.0f);

		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			UEdGraphNode* Node = *It;
			AvgNodePosition.X += Node->NodePosX;
			AvgNodePosition.Y += Node->NodePosY;
		}

		float InvNumNodes = 1.0f / float(PastedNodes.Num());
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;

		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			UEdGraphNode* Node = *It;
			CurrentGraphEditor->SetNodeSelection(Node, true);

			Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + Location.X;
			Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + Location.Y;

			Node->SnapToGrid(16);

			// Give new node a different Guid from the old one
			Node->CreateNewGuid();
		}
	}

	// Update UI
	CurrentGraphEditor->NotifyGraphChanged();

	UObject* GraphOwner = EdGraph->GetOuter();
	if (GraphOwner)
	{
		GraphOwner->PostEditChange();
		GraphOwner->MarkPackageDirty();
	}
}
bool FAssetEditor_QuestSystem::CanPasteNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return false;
	}

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(CurrentGraphEditor->GetCurrentGraph(), ClipboardContent);
}
void FAssetEditor_QuestSystem::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}
bool FAssetEditor_QuestSystem::CanDuplicateNodes()
{
	return CanCopyNodes();
}
void FAssetEditor_QuestSystem::CreateNewTask()
{
	HandleNewNodeClassPicked(UQuestSystemGraphNode::StaticClass());
}

void FAssetEditor_QuestSystem::HandleNewNodeClassPicked(UClass* InClass) const
{
	/*UE_CLOG(InClass == nullptr, LogBehaviorTreeEditor, Error, TEXT("Trying to handle new node of NULL class for Behavior Treee %s ")
		, *GetNameSafe(BehaviorTree));*/

	if (EditingGraph != nullptr && InClass != nullptr && EditingGraph->GetOutermost())
	{
		const FString ClassName = FBlueprintEditorUtils::GetClassNameWithoutSuffix(InClass);

		FString PathName = EditingGraph->GetOutermost()->GetPathName();
		PathName = FPaths::GetPath(PathName);

		// Now that we've generated some reasonable default locations/names for the package, allow the user to have the final say
		// before we create the package and initialize the blueprint inside of it.
		FSaveAssetDialogConfig SaveAssetDialogConfig;
		SaveAssetDialogConfig.DialogTitleOverride = LOCTEXT("SaveAssetDialogTitle", "Save Asset As");
		SaveAssetDialogConfig.DefaultPath = PathName;
		SaveAssetDialogConfig.DefaultAssetName = ClassName + TEXT("_New");
		SaveAssetDialogConfig.ExistingAssetPolicy = ESaveAssetDialogExistingAssetPolicy::Disallow;

		const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		const FString SaveObjectPath = ContentBrowserModule.Get().CreateModalSaveAssetDialog(SaveAssetDialogConfig);
		if (!SaveObjectPath.IsEmpty())
		{
			const FString SavePackageName = FPackageName::ObjectPathToPackageName(SaveObjectPath);
			const FString SavePackagePath = FPaths::GetPath(SavePackageName);
			const FString SaveAssetName = FPaths::GetBaseFilename(SavePackageName);

			UPackage* Package = CreatePackage(*SavePackageName);
			if (ensure(Package))
			{
				// Create and init a new Blueprint
				if (UBlueprint* NewBP = FKismetEditorUtilities::CreateBlueprint(InClass, Package, FName(*SaveAssetName), BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass()))
				{
					GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(NewBP);

					// Notify the asset registry
					FAssetRegistryModule::AssetCreated(NewBP);

					// Mark the package dirty...
					Package->MarkPackageDirty();
				}
			}
		}
	}

	FSlateApplication::Get().DismissAllMenus();
}

bool FAssetEditor_QuestSystem::CanCreateNewTask() const
{
	return true;
}
bool FAssetEditor_QuestSystem::IsNewTaskButtonVisible() const
{
	return true;
}

void FAssetEditor_QuestSystem::AutoArrange()
{
	UE_LOG(LogTemp, Log, TEXT("Click"))
}
bool FAssetEditor_QuestSystem::CanAutoArrange() const
{
	return true;
}
void FAssetEditor_QuestSystem::OnRenameNode()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
		{
			UEdGraphNode* SelectedNode = Cast<UEdGraphNode>(*NodeIt);
			if (SelectedNode != NULL && SelectedNode->bCanRenameNode)
			{
				CurrentGraphEditor->IsNodeTitleVisible(SelectedNode, true);
				break;
			}
		}
	}
}
bool FAssetEditor_QuestSystem::CanRenameNodes() const
{
	UEdGraph_QuestSystemGraph* EdGraph = Cast<UEdGraph_QuestSystemGraph>(EditingGraph->EdGraph);
	check(EdGraph != nullptr);

	UQuestSystemGraph* Graph = EdGraph->GetQuestSystemGraph();
	check(Graph != nullptr)

		return Graph->bCanRenameNode && GetSelectedNodes().Num() == 1;
}
void FAssetEditor_QuestSystem::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
	TArray<UObject*> Selection;

	for (UObject* SelectionEntry : NewSelection)
	{
		Selection.Add(SelectionEntry);
	}

	if (Selection.Num() == 0)
	{
		PropertyWidget->SetObject(EditingGraph);

	}
	else
	{
		PropertyWidget->SetObjects(Selection);
	}
}
void FAssetEditor_QuestSystem::OnNodeDoubleClicked(UEdGraphNode* Node)
{
}
void FAssetEditor_QuestSystem::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (EditingGraph == nullptr)
		return;

	EditingGraph->EdGraph->GetSchema()->ForceVisualizationCacheClear();
}
//Called when saving our file graph
#if ENGINE_MAJOR_VERSION < 5

void FAssetEditor_QuestSystem::OnPackageSaved(const FString& PackageFileName, UObject* Outer)
{
	RebuildGenericGraph();
}
#else // #if ENGINE_MAJOR_VERSION < 5
void FAssetEditor_QuestSystem::OnPackageSavedWithContext(const FString& PackageFileName, UPackage* Package, FObjectPostSaveContext ObjectSaveContext)
{
	RebuildQuestSystemGraph();
}
#endif // #else // #if ENGINE_MAJOR_VERSION < 5

#undef LOCTEXT_NAMESPACE
