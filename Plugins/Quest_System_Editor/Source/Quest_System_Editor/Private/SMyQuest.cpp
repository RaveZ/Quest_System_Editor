// Fill out your copyright notice in the Description page of Project Settings.


#include "SMyQuest.h"
#include "Framework/Commands/GenericCommands.h"
#include "GraphEditorActions.h"
#include "SGraphActionMenu.h"
#include "Widgets/Input/SSearchBox.h"
#include "SlateOptMacros.h"


#define LOCTEXT_NAMESPACE "MyQuest"

void FMyQuestCommands::RegisterCommands()
{
	UI_COMMAND(OpenGraph, "Open Graph", "Opens up this function, macro, or event graph's graph panel up.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenGraphInNewTab, "Open in New Tab", "Opens up this function, macro, or event graph's graph panel up in a new tab. Hold down Ctrl and double click for shortcut.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenExternalGraph, "Open External Graph", "Opens up this external graph's graph panel in its own asset editor", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(FocusNode, "Focus", "Focuses on the associated node", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(FocusNodeInNewTab, "Focus in New Tab", "Focuses on the associated node in a new tab", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ImplementFunction, "Implement event", "Implements this overridable function as a new event.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(DeleteEntry, "Delete", "Deletes this function or variable from this blueprint.", EUserInterfaceActionType::Button, FInputChord(EKeys::Platform_Delete));
	UI_COMMAND(PasteVariable, "Paste Variable", "Pastes the variable to this blueprint.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PasteLocalVariable, "Paste Local Variable", "Pastes the variable to this scope.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PasteFunction, "Paste Function", "Pastes the function to this blueprint.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PasteMacro, "Paste Macro", "Pastes the macro to this blueprint.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(GotoNativeVarDefinition, "Goto Code Definition", "Goto the native code definition of this variable", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(MoveVariableToParent, "Move to Parent Class", "Moves the variable to its parent class", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(MoveFunctionToParent, "Move to Parent Class", "Moves the function to its parent class", EUserInterfaceActionType::Button, FInputChord());
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMyQuest::Construct(const FArguments& InArgs, TWeakPtr<FAssetEditor_QuestSystem> InQuestEditor, const UQuestSystemGraph* InQuestSystemGraph)
{
	bNeedsRefresh = false;
	bShowReplicatedVariablesOnly = false;

	QuestEditorPtr = InQuestEditor;
	EdGraph = nullptr;

	TSharedPtr<SWidget> ToolbarBuilderWidget = TSharedPtr<SWidget>();

	if (InQuestEditor.IsValid())
	{
		QuestSystemGraph = QuestEditorPtr.Pin()->GetQuestSystemObj();

		CommandList = MakeShareable(new FUICommandList);

		CommandList->Append(InQuestEditor.Pin()->GetToolkitCommands());

		CommandList->MapAction(FMyQuestCommands::Get().OpenGraph,
			FExecuteAction::CreateSP(this, &SMyQuest::OnOpenGraph),
			FCanExecuteAction(), FIsActionChecked(),
			FIsActionButtonVisible::CreateSP(this, &SMyQuest::CanOpenGraph));

		CommandList->MapAction(FMyQuestCommands::Get().OpenGraphInNewTab,
			FExecuteAction::CreateSP(this, &SMyQuest::OnOpenGraphInNewTab),
			FCanExecuteAction(), FIsActionChecked(),
			FIsActionButtonVisible::CreateSP(this, &SMyQuest::CanOpenGraph));

		CommandList->MapAction(FMyQuestCommands::Get().OpenExternalGraph,
			FExecuteAction::CreateSP(this, &SMyQuest::OnOpenExternalGraph),
			FCanExecuteAction(), FIsActionChecked(),
			FIsActionButtonVisible::CreateSP(this, &SMyQuest::CanOpenExternalGraph));

		CommandList->MapAction(FMyQuestCommands::Get().FocusNode,
			FExecuteAction::CreateSP(this, &SMyQuest::OnFocusNode),
			FCanExecuteAction(), FIsActionChecked(),
			FIsActionButtonVisible::CreateSP(this, &SMyQuest::CanFocusOnNode));

		CommandList->MapAction(FMyQuestCommands::Get().FocusNodeInNewTab,
			FExecuteAction::CreateSP(this, &SMyQuest::OnFocusNodeInNewTab),
			FCanExecuteAction(), FIsActionChecked(),
			FIsActionButtonVisible::CreateSP(this, &SMyQuest::CanFocusOnNode));


		CommandList->MapAction(FMyQuestCommands::Get().DeleteEntry,
			FExecuteAction::CreateSP(this, &SMyQuest::OnDeleteEntry),
			FCanExecuteAction::CreateSP(this, &SMyQuest::CanDeleteEntry));

		CommandList->MapAction(FGenericCommands::Get().Duplicate,
			FExecuteAction::CreateSP(this, &SMyQuest::OnDuplicateAction),
			FCanExecuteAction::CreateSP(this, &SMyQuest::CanDuplicateAction),
			FIsActionChecked(),
			FIsActionButtonVisible::CreateSP(this, &SMyQuest::IsDuplicateActionVisible));

		CommandList->MapAction(FMyQuestCommands::Get().MoveVariableToParent,
			FExecuteAction::CreateSP(this, &SMyQuest::OnMoveToParent),
			FCanExecuteAction(),
			FIsActionChecked(),
			FIsActionButtonVisible::CreateSP(this, &SMyQuest::CanMoveVariableToParent));

		CommandList->MapAction(FMyQuestCommands::Get().MoveFunctionToParent,
			FExecuteAction::CreateSP(this, &SMyQuest::OnMoveToParent),
			FCanExecuteAction(),
			FIsActionChecked(),
			FIsActionButtonVisible::CreateSP(this, &SMyQuest::CanMoveFunctionToParent));

		CommandList->MapAction(FMyQuestCommands::Get().GotoNativeVarDefinition,
			FExecuteAction::CreateSP(this, &SMyQuest::GotoNativeCodeVarDefinition),
			FCanExecuteAction(),
			FIsActionChecked(),
			FIsActionButtonVisible::CreateSP(this, &SMyQuest::IsNativeVariable));

		ToolbarBuilderWidget = SNullWidget::NullWidget;

		CommandList->MapAction(FGenericCommands::Get().Rename,
			FExecuteAction::CreateSP(this, &SMyQuest::OnRequestRenameOnActionNode),
			FCanExecuteAction::CreateSP(this, &SMyQuest::CanRequestRenameOnActionNode));

		CommandList->MapAction(FGenericCommands::Get().Copy,
			FExecuteAction::CreateSP(this, &SMyQuest::OnCopy),
			FCanExecuteAction::CreateSP(this, &SMyQuest::CanCopy));

		CommandList->MapAction(FGenericCommands::Get().Cut,
			FExecuteAction::CreateSP(this, &SMyQuest::OnCut),
			FCanExecuteAction::CreateSP(this, &SMyQuest::CanCut));

		CommandList->MapAction(FGenericCommands::Get().Paste,
			FExecuteAction::CreateSP(this, &SMyQuest::OnPasteGeneric),
			FCanExecuteAction(), FIsActionChecked(),
			FIsActionButtonVisible::CreateSP(this, &SMyQuest::CanPasteGeneric));

	}
	else
	{
		// we're in read only mode when there's no quest editor:
		QuestSystemGraph = const_cast<UQuestSystemGraph*>(InQuestSystemGraph);
		check(QuestSystemGraph);
		ToolbarBuilderWidget = SNew(SBox);
	}


	SAssignNew(FilterBox, SSearchBox)
		.OnTextChanged(this, &SMyQuest::OnFilterTextChanged);

	// create the main action list piece of this widget
	SAssignNew(GraphActionMenu, SGraphActionMenu, false)
		.OnGetFilterText(this, &SMyQuest::GetFilterText)
		.OnCreateWidgetForAction(this, &SMyQuest::OnCreateWidgetForAction)
		.OnCollectAllActions(this, &SMyQuest::CollectAllActions)
		.OnCollectStaticSections(this, &SMyQuest::CollectStaticSections)
		.OnActionDragged(this, &SMyQuest::OnActionDragged)
		.OnCategoryDragged(this, &SMyQuest::OnCategoryDragged)
		.OnActionSelected(this, &SMyQuest::OnGlobalActionSelected)
		.OnActionDoubleClicked(this, &SMyQuest::OnActionDoubleClicked)
		.OnContextMenuOpening(this, &SMyQuest::OnContextMenuOpening)
		.OnCategoryTextCommitted(this, &SMyQuest::OnCategoryNameCommitted)
		.OnCanRenameSelectedAction(this, &SMyQuest::CanRequestRenameOnActionNode)
		.OnGetSectionTitle(this, &SMyQuest::OnGetSectionTitle)
		.OnGetSectionWidget(this, &SMyQuest::OnGetSectionWidget)
		.OnActionMatchesName(this, &SMyQuest::HandleActionMatchesName)
		.AlphaSortItems(false)
		.UseSectionStyling(true);

	ChildSlot
	[
		SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
					.Padding(4.0f)
					.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
					.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("MyQuestPanel")))
					[
						SNew(SVerticalBox)

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							ToolbarBuilderWidget.ToSharedRef()
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SHorizontalBox)

							+ SHorizontalBox::Slot()
							.FillWidth(1.0f)
							.VAlign(VAlign_Center)
							[
								FilterBox.ToSharedRef()
							]
						]
					]
			]

			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				GraphActionMenu.ToSharedRef()
			]
	];

	//ResetLastPinType();

	if (!QuestEditorPtr.IsValid())
	{
		//Refresh();
	}

	TMap<int32, bool> ExpandedSections;
	ExpandedSections.Add(QuestSectionID::QUESTGRAPH, true);

	GraphActionMenu->SetSectionExpansion(ExpandedSections);

	//FCoreUObjectDelegates::OnObjectPropertyChanged.AddRaw(this, &SMyQuest::OnObjectPropertyChanged);

}

SMyQuest::~SMyQuest()
{

}
void SMyQuest::OnRequestRenameOnActionNode()
{
}
void SMyQuest::OnPasteGeneric()
{
}
bool SMyQuest::CanPasteGeneric()
{
	return false;
}
TSharedRef<SWidget> SMyQuest::OnCreateWidgetForAction(FCreateWidgetForActionData* const InCreateData)
{
	return SNew(SBox);
}
void SMyQuest::CollectAllActions(FGraphActionListBuilderBase& OutAllActions)
{
}
void SMyQuest::CollectStaticSections(TArray<int32>& StaticSectionIDs)
{
	StaticSectionIDs.Add(QuestSectionID::QUESTGRAPH);
}
FReply SMyQuest::OnActionDragged(const TArray<TSharedPtr<FEdGraphSchemaAction>>& InActions, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}
FReply SMyQuest::OnCategoryDragged(const FText& InCategory, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}
void SMyQuest::OnGlobalActionSelected(const TArray<TSharedPtr<FEdGraphSchemaAction>>& InActions, ESelectInfo::Type InSelectionType)
{
}
void SMyQuest::OnActionDoubleClicked(const TArray<TSharedPtr<FEdGraphSchemaAction>>& InActions)
{
}
TSharedPtr<SWidget> SMyQuest::OnContextMenuOpening()
{
	return SNew(SBox);
}
TSharedRef<SWidget> SMyQuest::CreateAddToSectionButton(int32 InSectionID, TWeakPtr<SWidget> WeakRowWidget, FText AddNewText, FName MetaDataTag)
{
	return
		SNew(SButton)
		.ButtonStyle(FAppStyle::Get(), "SimpleButton")
		.OnClicked(this, &SMyQuest::OnAddButtonClickedOnSection, InSectionID)
		.IsEnabled(this, &SMyQuest::CanAddNewElementToSection, InSectionID)
		.ContentPadding(FMargin(1, 0))
		.AddMetaData<FTagMetaData>(FTagMetaData(MetaDataTag))
		.ToolTipText(AddNewText)
		[
			SNew(SImage)
				.Image(FAppStyle::Get().GetBrush("Icons.PlusCircle"))
				.ColorAndOpacity(FSlateColor::UseForeground())
		];
}
void SMyQuest::OnCategoryNameCommitted(const FText& InNewText, ETextCommit::Type InTextCommit, TWeakPtr<struct FGraphActionNode> InAction)
{
}
bool SMyQuest::CanRequestRenameOnActionNode(TWeakPtr<struct FGraphActionNode> InSelectedNode) const
{
	return false;
}
FText SMyQuest::OnGetSectionTitle(int32 InSectionID)
{
	FText SeperatorTitle;
	/* Setup an appropriate name for the section for this node */
	switch (InSectionID)
	{
	case QuestSectionID::QUESTGRAPH:
		SeperatorTitle = NSLOCTEXT("GraphActionNode", "QuestGraphs", "QuestGraphs");
		break; 
	default:
	case QuestSectionID::NONE:
		SeperatorTitle = FText::GetEmpty();
		break;
	}
	
	return SeperatorTitle;
}
TSharedRef<SWidget> SMyQuest::OnGetSectionWidget(TSharedRef<SWidget> RowWidget, int32 InSectionID)
{
	TWeakPtr<SWidget> WeakRowWidget = RowWidget;

	FText AddNewText;
	FName MetaDataTag;

	switch (InSectionID)
	{
	case QuestSectionID::QUESTGRAPH:
		AddNewText = LOCTEXT("AddNewQuestGraph", "New Quest Graph");
		MetaDataTag = TEXT("AddNewGraph");
		break;
	default:
		return SNullWidget::NullWidget;
	}

	return CreateAddToSectionButton(InSectionID, WeakRowWidget, AddNewText, MetaDataTag);
}
FReply SMyQuest::OnAddButtonClickedOnSection(int32 InSectionID)
{
	return FReply::Handled();
}
bool SMyQuest::CanAddNewElementToSection(int32 InSectionID) const
{
	return false;
}
bool SMyQuest::HandleActionMatchesName(FEdGraphSchemaAction* InAction, const FName& InName) const
{
	return false;
}
void SMyQuest::OnOpenGraph()
{
}
void SMyQuest::OnOpenGraphInNewTab()
{
}
void SMyQuest::OnOpenExternalGraph()
{
}
bool SMyQuest::CanOpenGraph() const
{
	return false;
}
bool SMyQuest::CanOpenExternalGraph() const
{
	return false;
}
bool SMyQuest::CanFocusOnNode() const
{
	return false;
}
void SMyQuest::OnFocusNode()
{
}
void SMyQuest::OnFocusNodeInNewTab()
{
}
void SMyQuest::OnDeleteEntry()
{
}
bool SMyQuest::CanDeleteEntry() const
{
	return false;
}
bool SMyQuest::CanRequestRenameOnActionNode() const
{
	return false;
}
bool SMyQuest::IsDuplicateActionVisible() const
{
	return false;
}
bool SMyQuest::CanDuplicateAction() const
{
	return false;
}
void SMyQuest::OnDuplicateAction()
{
}
void SMyQuest::GotoNativeCodeVarDefinition()
{
}
bool SMyQuest::IsNativeVariable() const
{
	return false;
}
void SMyQuest::OnMoveToParent()
{
}
bool SMyQuest::CanMoveVariableToParent() const
{
	return false;
}
bool SMyQuest::CanMoveFunctionToParent() const
{
	return false;
}
void SMyQuest::OnCopy()
{
}
bool SMyQuest::CanCopy() const
{
	return false;
}
void SMyQuest::OnCut()
{
}
bool SMyQuest::CanCut() const
{
	return false;
}

void SMyQuest::OnFilterTextChanged(const FText& InFilterText)
{
}
FText SMyQuest::GetFilterText() const
{
	return FText();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION



#undef LOCTEXT_NAMESPACE