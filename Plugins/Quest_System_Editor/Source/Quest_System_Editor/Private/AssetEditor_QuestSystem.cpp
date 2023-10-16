// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetEditor_QuestSystem.h"
#include "UObject/ObjectSaveContext.h"
#include "GraphEditorActions.h"
#include "Framework/Commands/GenericCommands.h"
#include "Quest_System_EditorCommands.h"
#include "EdGraph_QuestSystemGraph.h"
#include "AssetGraphSchema_QuestSystem.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "AssetEditor_GenericGraph"

struct FGenericGraphAssetEditorTabs
{
	// Tab identifiers
	static const FName GenericGraphPropertyID;
	static const FName ViewportID;
	static const FName GenericGraphEditorSettingsID;
};

//////////////////////////////////////////////////////////////////////////

const FName FGenericGraphAssetEditorTabs::GenericGraphPropertyID(TEXT("GenericGraphProperty"));
const FName FGenericGraphAssetEditorTabs::ViewportID(TEXT("Viewport"));
const FName FGenericGraphAssetEditorTabs::GenericGraphEditorSettingsID(TEXT("GenericGraphEditorSettings"));

//////////////////////////////////////////////////////////////////////////

const FName GenericGraphEditorAppName = FName(TEXT("GenericGraphEditorApp"));

FAssetEditor_QuestSystem::FAssetEditor_QuestSystem()
{
	EditingGraph = nullptr;

	//GenricGraphEditorSettings = NewObject<UGenericGraphEditorSettings>(UGenericGraphEditorSettings::StaticClass());

#if ENGINE_MAJOR_VERSION < 5
	OnPackageSavedDelegateHandle = UPackage::PackageSavedEvent.AddRaw(this, &FAssetEditor_GenericGraph::OnPackageSaved);
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

	/*if (!ToolbarBuilder.IsValid())
	{
		ToolbarBuilder = MakeShareable(new FAssetEditorToolbar_GenericGraph(SharedThis(this)));
	}*/

	//BindCommands();

	CreateInternalWidgets();

	//TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	//ToolbarBuilder->AddGenericGraphToolbar(ToolbarExtender);

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
					->AddTab(FGenericGraphAssetEditorTabs::ViewportID, ETabState::OpenedTab)->SetHideTabWell(true)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.7f)
						->AddTab(FGenericGraphAssetEditorTabs::GenericGraphPropertyID, ETabState::OpenedTab)->SetHideTabWell(true)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.3f)
						->AddTab(FGenericGraphAssetEditorTabs::GenericGraphEditorSettingsID, ETabState::OpenedTab)
					)
				)
			)
		);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, GenericGraphEditorAppName, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, EditingGraph, false);

	//RegenerateMenusAndToolbars();
}

void FAssetEditor_QuestSystem::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_GenericGraphEditor", "Generic Graph Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FGenericGraphAssetEditorTabs::ViewportID, FOnSpawnTab::CreateSP(this, &FAssetEditor_QuestSystem::SpawnTab_Viewport))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(FGenericGraphAssetEditorTabs::GenericGraphPropertyID, FOnSpawnTab::CreateSP(this, &FAssetEditor_QuestSystem::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTab", "Property"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));

	/*InTabManager->RegisterTabSpawner(FGenericGraphAssetEditorTabs::GenericGraphEditorSettingsID, FOnSpawnTab::CreateSP(this, &FAssetEditor_QuestSystem::SpawnTab_EditorSettings))
		.SetDisplayName(LOCTEXT("EditorSettingsTab", "Generic Graph Editor Setttings"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));*/
}

void FAssetEditor_QuestSystem::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FGenericGraphAssetEditorTabs::ViewportID);
	InTabManager->UnregisterTabSpawner(FGenericGraphAssetEditorTabs::GenericGraphPropertyID);
	InTabManager->UnregisterTabSpawner(FGenericGraphAssetEditorTabs::GenericGraphEditorSettingsID);
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

FName FAssetEditor_QuestSystem::GetToolkitFName() const
{
	return FName();
}

FText FAssetEditor_QuestSystem::GetBaseToolkitName() const
{
	return FText();
}

FText FAssetEditor_QuestSystem::GetToolkitName() const
{
	return FText();
}

FText FAssetEditor_QuestSystem::GetToolkitToolTipText() const
{
	return FText();
}

FLinearColor FAssetEditor_QuestSystem::GetWorldCentricTabColorScale() const
{
	return FLinearColor();
}

FString FAssetEditor_QuestSystem::GetWorldCentricTabPrefix() const
{
	return FString();
}

FString FAssetEditor_QuestSystem::GetDocumentationLink() const
{
	return FString();
}

void FAssetEditor_QuestSystem::SaveAsset_Execute()
{
	if (EditingGraph != nullptr)
	{
		RebuildQuestSystemGraph();
	}

	FAssetEditorToolkit::SaveAsset_Execute();
}

void FAssetEditor_QuestSystem::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditingGraph);
	Collector.AddReferencedObject(EditingGraph->EdGraph);
}

TSharedRef<SDockTab> FAssetEditor_QuestSystem::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FGenericGraphAssetEditorTabs::ViewportID);

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
	check(Args.GetTabId() == FGenericGraphAssetEditorTabs::GenericGraphPropertyID);

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
	check(Args.GetTabId() == FGenericGraphAssetEditorTabs::GenericGraphEditorSettingsID);

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

	/*EditorSettingsWidget = PropertyModule.CreateDetailView(Args);
	EditorSettingsWidget->SetObject(GenricGraphEditorSettings);*/
}

TSharedRef<SGraphEditor> FAssetEditor_QuestSystem::CreateViewportWidget()
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_GenericGraph", "Generic Graph");

	//CreateCommandList();

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
void FAssetEditor_QuestSystem::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
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
