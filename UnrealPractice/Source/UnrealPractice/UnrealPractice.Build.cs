// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealPractice : ModuleRules
{
	public UnrealPractice(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"SlateCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"UnrealPractice",
			"UnrealPractice/Variant_Platforming",
			"UnrealPractice/Variant_Platforming/Animation",
			"UnrealPractice/Variant_Combat",
			"UnrealPractice/Variant_Combat/AI",
			"UnrealPractice/Variant_Combat/Animation",
			"UnrealPractice/Variant_Combat/Gameplay",
			"UnrealPractice/Variant_Combat/Interfaces",
			"UnrealPractice/Variant_Combat/UI",
			"UnrealPractice/Variant_SideScrolling",
			"UnrealPractice/Variant_SideScrolling/AI",
			"UnrealPractice/Variant_SideScrolling/Gameplay",
			"UnrealPractice/Variant_SideScrolling/Interfaces",
			"UnrealPractice/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
