// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestEditorFactory.h"
#include "QuestSystemGraph.h"

#include "AssetEditor_QuestSystem.h"
#include "ClassViewerModule.h"
#include "ClassViewerFilter.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/SClassPickerDialog.h"

#define LOCTEXT_NAMESPACE "Quest_Editor"

class FAssetClassParentFilter : public IClassViewerFilter
{
public:
	FAssetClassParentFilter()
		: DisallowedClassFlags(CLASS_None), bDisallowBlueprintBase(false)
	{}

	/** All children of these classes will be included unless filtered out by another setting. */
	TSet< const UClass* > AllowedChildrenOfClasses;

	/** Disallowed class flags. */
	EClassFlags DisallowedClassFlags;

	/** Disallow blueprint base classes. */
	bool bDisallowBlueprintBase;

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		bool bAllowed = !InClass->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;

		if (bAllowed && bDisallowBlueprintBase)
		{
			if (FKismetEditorUtilities::CanCreateBlueprintOfClass(InClass))
			{
				return false;
			}
		}

		return bAllowed;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		if (bDisallowBlueprintBase)
		{
			return false;
		}

		return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
	}
};

UQuestEditorFactory::UQuestEditorFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UQuestSystemGraph::StaticClass();
}

UQuestEditorFactory::~UQuestEditorFactory()
{
	
}


bool UQuestEditorFactory::ConfigureProperties()
{
	//add class picker
//	// nullptr the GenericGraphClass so we can check for selection
//	QuestSystemGraphClass = nullptr;
//
//	// Load the classviewer module to display a class picker
//	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
//
//	// Fill in options
//	FClassViewerInitializationOptions Options;
//	Options.Mode = EClassViewerMode::ClassPicker;
//
//#if ENGINE_MAJOR_VERSION < 5
//	TSharedPtr<FAssetClassParentFilter> Filter = MakeShareable(new FAssetClassParentFilter);
//	Options.ClassFilter = Filter;
//#else // #if ENGINE_MAJOR_VERSION < 5
//	TSharedRef<FAssetClassParentFilter> Filter = MakeShareable(new FAssetClassParentFilter);
//	Options.ClassFilters.Add(Filter);
//#endif // #else // #if ENGINE_MAJOR_VERSION < 5
//
//	Filter->DisallowedClassFlags = CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_HideDropDown;
//	Filter->AllowedChildrenOfClasses.Add(UQuestSystemGraph::StaticClass());
//
//	const FText TitleText = LOCTEXT("CreateQuestSystemGraphAssetOptions", "Pick Quest System Class");
//	UClass* ChosenClass = nullptr;
//	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UQuestSystemGraph::StaticClass());
//
//	if (bPressedOk)
//	{
//		QuestSystemGraphClass = ChosenClass;
//	}
//
//	return bPressedOk;

	return true;
}

UObject* UQuestEditorFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	if (QuestSystemGraphClass != nullptr)
	{
		return NewObject<UQuestSystemGraph>(InParent, QuestSystemGraphClass, Name, Flags | RF_Transactional);
	}
	else
	{
		check(Class->IsChildOf(UQuestSystemGraph::StaticClass()));
		return NewObject<UObject>(InParent, Class, Name, Flags | RF_Transactional);
	}
}

FQuestGraphEditorSummoner::FQuestGraphEditorSummoner(TSharedPtr<class FAssetEditor_QuestSystem> InQuestEditorPtr, FOnCreateGraphEditorWidget CreateGraphEditorWidgetCallback) : FDocumentTabFactoryForObjects<UEdGraph>(FQuestSystemAssetEditorTabs::ViewportID, InQuestEditorPtr)
, QuestEditorPtr(InQuestEditorPtr)
, OnCreateGraphEditorWidget(CreateGraphEditorWidgetCallback)
{
}

void FQuestGraphEditorSummoner::OnTabActivated(TSharedPtr<SDockTab> Tab) const
{
	TSharedRef<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(Tab->GetContent());
	QuestEditorPtr.Pin()->OnGraphEditorFocused(GraphEditor);
}

void FQuestGraphEditorSummoner::OnTabBackgrounded(TSharedPtr<SDockTab> Tab) const
{
	TSharedRef<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(Tab->GetContent());
	QuestEditorPtr.Pin()->OnGraphEditorBackgrounded(GraphEditor);
}

void FQuestGraphEditorSummoner::OnTabRefreshed(TSharedPtr<SDockTab> Tab) const
{
	TSharedRef<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(Tab->GetContent());
	GraphEditor->NotifyGraphChanged();
}

void FQuestGraphEditorSummoner::SaveState(TSharedPtr<SDockTab> Tab, TSharedPtr<FTabPayload> Payload) const
{
	TSharedRef<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(Tab->GetContent());

	FVector2D ViewLocation;
	float ZoomAmount;
	GraphEditor->GetViewLocation(ViewLocation, ZoomAmount);

	UEdGraph* Graph = Payload->IsValid() ? FTabPayload_UObject::CastChecked<UEdGraph>(Payload) : nullptr;

	if (Graph && QuestEditorPtr.Pin()->IsGraphInCurrentQuestGraph(Graph))
	{
		// Don't save references to external graphs.
		QuestEditorPtr.Pin()->GetQuestSystemObj()->LastEditedDocuments.Add(FEditedDocumentInfo(Graph, ViewLocation, ZoomAmount));
	}
}

TSharedRef<SWidget> FQuestGraphEditorSummoner::CreateTabBodyForObject(const FWorkflowTabSpawnInfo& Info, UEdGraph* DocumentID) const
{
	check(Info.TabInfo.IsValid());
	return OnCreateGraphEditorWidget.Execute(Info.TabInfo.ToSharedRef(), DocumentID);
}

const FSlateBrush* FQuestGraphEditorSummoner::GetTabIconForObject(const FWorkflowTabSpawnInfo& Info, UEdGraph* DocumentID) const
{
	return FAssetEditor_QuestSystem::GetGlyphForGraph(DocumentID, false);
}






#undef LOCTEXT_NAMESPACE


