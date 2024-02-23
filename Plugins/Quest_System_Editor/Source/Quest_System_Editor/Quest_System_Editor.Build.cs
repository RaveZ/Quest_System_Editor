// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Quest_System_Editor : ModuleRules
{
	public Quest_System_Editor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                "Quest_System_Editor/Private",
                "Quest_System_Editor/Public",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Quest_System_Runtime",
                "Core",
                "CoreUObject",
                "Engine",
                "UnrealEd",
				"Kismet",
				"AIGraph",
				"EditorStyle",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Quest_System_Runtime",
                "AssetTools",
                "GraphEditor",
                "PropertyEditor",
                "EditorStyle",
                "Kismet",
                "KismetWidgets",
                "ApplicationCore",
                "ToolMenus",
                "Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "AIGraph",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
