// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemEditorUtils.h"

FQuestSystemEditorUtils::FQuestSystemEditorUtils()
{
}

FQuestSystemEditorUtils::~FQuestSystemEditorUtils()
{
}

FName FQuestSystemEditorUtils::FindUniqueQuestName(const UQuestSystemGraph* InQuestSystemGraph, const FString& InBaseName, UStruct* InScope)
{
	return FName();
}

UEdGraph* FQuestSystemEditorUtils::CreateNewGraph(UObject* ParentScope, const FName& GraphName, TSubclassOf<class UEdGraph> GraphClass, TSubclassOf<class UEdGraphSchema> SchemaClass)
{
	UEdGraph* NewGraph = nullptr;
	bool bRename = false;

	// Ensure this name isn't already being used for a graph
	if (GraphName != NAME_None)
	{
		if (UObject* ExistingObject = FindObject<UObject>(ParentScope, *(GraphName.ToString())))
		{
			if (ExistingObject->IsA<UEdGraph>())
			{
				// Rename the old graph out of the way - this may confuse the user somewhat - and even
				// break their logic. But name collisions are not avoidable e.g. someone can add
				// a function to an interface that conflicts with something in a class hierarchy
				ExistingObject->Rename(nullptr, ExistingObject->GetOuter(), REN_DoNotDirty | REN_ForceNoResetLoaders);
			}
			else if (ExistingObject->IsA<UObjectRedirector>())
			{
				const UBlueprint* Blueprint = Cast<UBlueprint>(ParentScope);
				if (Blueprint && Blueprint->BlueprintType == BPTYPE_MacroLibrary)
				{
					// When renaming a graph inside a macro library, we may have dropped a redirector after a previous
					// rename (see RenameGraph). If we're now reusing it, move the redirector aside to free up the name.
					ExistingObject->Rename(nullptr, GetTransientPackage(), REN_DontCreateRedirectors);
				}
			}
		}

		// Construct new graph with the supplied name
		NewGraph = NewObject<UEdGraph>(ParentScope, GraphClass, NAME_None, RF_Transactional);
		bRename = true;
	}
	else
	{
		// Construct a new graph with a default name
		NewGraph = NewObject<UEdGraph>(ParentScope, GraphClass, NAME_None, RF_Transactional);
	}

	NewGraph->Schema = SchemaClass;

	// Now move to where we want it to. Workaround to ensure transaction buffer is correctly utilized
	if (bRename)
	{
		NewGraph->Rename(*(GraphName.ToString()), ParentScope, REN_DoNotDirty | REN_ForceNoResetLoaders);
	}
	return NewGraph;
}

void FQuestSystemEditorUtils::AddQuestGraphPage(UQuestSystemGraph* QuestSystemGraph, UEdGraph* Graph)
{
#if WITH_EDITORONLY_DATA
	QuestSystemGraph->QuestGraphPages.Add(Graph);
	Graph->MarkPackageDirty();
#endif	//#if WITH_EDITORONLY_DATA
}
