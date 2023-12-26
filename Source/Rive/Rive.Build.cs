// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Rive : ModuleRules
{
	public Rive(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "Projects",
                "RHICore",
				"RenderCore",
				"RiveCore",
                "RiveLibrary",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"RiveRenderer",
				"RHI",
				"RenderCore",
				"Renderer",
                "RiveCore",
                "RiveLibrary",
                "Slate",
                "SlateCore",
                "UMG",
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
