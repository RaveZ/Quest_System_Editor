// Copyright Epic Games, Inc. All Rights Reserved.

#include "Quest_System_EditorCommands.h"

#define LOCTEXT_NAMESPACE "FQuest_System_EditorModule"

void FQuest_System_EditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Quest_System_Editor", "Bring up Quest_System_Editor window", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(GraphSettings, "Graph Settings", "Graph Settings", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AutoArrange, "Auto Arrange", "Auto Arrange", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
