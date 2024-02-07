// Copyright Rive, Inc. All rights reserved.

using EpicGames.Core;
using System.IO;
using UnrealBuildTool;

public class RiveLibrary : ModuleRules
{
	public RiveLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        bool bIsPlatformAdded = false;

        PrivateDependencyModuleNames.Add("Vulkan");

        AddEngineThirdPartyPrivateStaticDependencies(Target, "Vulkan");


        AddEngineThirdPartyPrivateStaticDependencies(Target, "zlib");

        string RootDir = ModuleDirectory;

        string IncludePath = Path.Combine(RootDir, "Includes");

        PublicSystemIncludePaths.Add(IncludePath);

        // NOTE: Incase if needed, otherwise feel free to remove it.
        bool bDebug = (Target.Configuration == UnrealTargetConfiguration.Debug && Target.bDebugBuildsActuallyUseDebugCRT);

        string LibPostfix = bDebug ? "_d" : "";

        if (Target.Platform.IsInGroup(UnrealPlatformGroup.Windows))
        {
            AddEngineThirdPartyPrivateStaticDependencies(Target, "DX9");

            AddEngineThirdPartyPrivateStaticDependencies(Target, "DX11");

            // AddEngineThirdPartyPrivateStaticDependencies(Target, "DX12");

            PublicSystemLibraries.Add("d3dcompiler.lib");

            string LibDirectory = Path.Combine(RootDir, "Libraries", "Win64");
            string RiveLibPng = "rive_libpng" + LibPostfix + ".lib";
            string RiveSheenBidiStaticLibName = "rive_sheenbidi" + LibPostfix + ".lib";
            string RiveHarfBuzzStaticLibName = "rive_harfbuzz" + LibPostfix + ".lib";
            // string RiveHarfBuzzDynamicLibName = "rive_harfbuzz" + LibPostfix + ".dll";
            string RiveStaticLibName = "rive" + LibPostfix + ".lib";
            string RiveDecodersStaticLibName = "rive_decoders" + LibPostfix + ".lib";
            string RivePlsLibName = "rive_pls_renderer" + LibPostfix + ".lib";

            PublicRuntimeLibraryPaths.Add(LibDirectory);
            PublicAdditionalLibraries.AddRange(new string[]
                {
                    Path.Combine(LibDirectory, RiveSheenBidiStaticLibName),
                    Path.Combine(LibDirectory, RiveHarfBuzzStaticLibName),
                    Path.Combine(LibDirectory, RiveStaticLibName),
                    Path.Combine(LibDirectory, RiveDecodersStaticLibName),
                    Path.Combine(LibDirectory, RivePlsLibName),
                    Path.Combine(LibDirectory, RiveLibPng)
                }
            );
            // PrivateRuntimeLibraryPaths.Add(LibDirectory);
            // PublicDelayLoadDLLs.Add(RiveHarfBuzzDynamicLibName);
            // RuntimeDependencies.Add(Path.Combine("$(TargetOutputDir)", RiveHarfBuzzDynamicLibName),Path.Combine(LibDirectory, RiveHarfBuzzDynamicLibName));
            // RuntimeDependencies.Add(RiveHarfBuzzDynamicLibName);

            bIsPlatformAdded = true;
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            string LibDirectory = Path.Combine(RootDir, "Libraries", "Mac");

            // NOTE : Link MacOS Libraries

            bIsPlatformAdded = true;
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.Add("OpenGLDrv");
            AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenGL");
            
            string LibDirectory = Path.Combine(RootDir, "Libraries", "Android");
            string RiveSheenBidiStaticLibName = "librive_sheenbidi" + LibPostfix + ".a";
            // string RiveHarfBuzzStaticLibName = "librive_harfbuzz" + LibPostfix + ".a";
            string RiveHarfBuzzSharedLibName = "librive_harfbuzz" + LibPostfix + ".so";
            string RiveStaticLibName = "librive" + LibPostfix + ".a"; ;
            string RiveDecodersStaticLibName = "librive_decoders" + LibPostfix + ".a";
            string RivePlsLibName = "librive_pls_renderer" + LibPostfix + ".a";
            string RiveLibPng = "liblibpng" + LibPostfix + ".a";

            PublicRuntimeLibraryPaths.Add(LibDirectory);
            PublicAdditionalLibraries.AddRange(new string[] 
                { 
                    Path.Combine(LibDirectory, RiveSheenBidiStaticLibName)
                    , Path.Combine(LibDirectory, RiveHarfBuzzSharedLibName)
                    , Path.Combine(LibDirectory, RiveStaticLibName)
                    , Path.Combine(LibDirectory, RiveDecodersStaticLibName)
                    , Path.Combine(LibDirectory, RivePlsLibName)
                    , Path.Combine(LibDirectory, RiveLibPng)
                }
            );
            RuntimeDependencies.Add(Path.Combine("$(TargetOutputDir)", RiveHarfBuzzSharedLibName),Path.Combine(LibDirectory, RiveHarfBuzzSharedLibName));
            
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "RiveLibrary_APL.xml"));
            
            PrecompileForTargets = PrecompileTargetsType.None;
            bIsPlatformAdded = true;
        }
        else if (Target.IsInPlatformGroup(UnrealPlatformGroup.Unix))
        {
            string LibDirectory = Path.Combine(RootDir, "Libraries", "Unix");

            // NOTE : Link Unix Libraries

            bIsPlatformAdded = true;
        }

        PublicDefinitions.Add("WITH_RIVE=" + (bIsPlatformAdded ? "1" : "0"));
    }
}
