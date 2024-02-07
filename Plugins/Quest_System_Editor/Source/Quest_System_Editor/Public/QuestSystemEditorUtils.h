// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraph.h"

/**
 * 
 */
class QUEST_SYSTEM_EDITOR_API FQuestSystemEditorUtils
{
public:
	FQuestSystemEditorUtils();
	~FQuestSystemEditorUtils();

	/**
	 * Finds a unique name with a base of the passed in string, appending numbers as needed
	 *
	 * @param InBlueprint		The blueprint the kismet object's name needs to be unique in
	 * @param InBaseName		The base name to use
	 * @param InScope			Scope, if any, of the unique kismet name to generate, used for locals
	 *
	 * @return					A unique name that will not conflict in the Blueprint
	 */
	static FName FindUniqueQuestName(const UQuestSystemGraph* InQuestSystemGraph, const FString& InBaseName, UStruct* InScope = NULL);


	/**
	 * Creates a new empty graph.
	 *
	 * @param	ParentScope		The outer of the new graph.
	 * @param	GraphName		Name of the graph to add.
	 * @param	SchemaClass		Schema to use for the new graph.
	 *
	 * @return	null if it fails, else.
	 */
	static class UEdGraph* CreateNewGraph(UObject* ParentScope, const FName& GraphName, TSubclassOf<class UEdGraph> GraphClass, TSubclassOf<class UEdGraphSchema> SchemaClass);

	/** Adds an questgraph page to this Quest Editor */
	static void AddQuestGraphPage(UQuestSystemGraph* InQuestSystemGraph, class UEdGraph* Graph);

	/** Returns the event graph, if any */
	static UEdGraph* FindQuestGraph(const UQuestSystemGraph* QuestSystemGraph);
};
