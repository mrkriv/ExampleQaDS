using UnrealBuildTool;

public class DialogPluginExample : ModuleRules
{
	public DialogPluginExample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        OptimizeCode = CodeOptimization.Never;

        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore" 
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{ 
			"RHI",
			"RenderCore",

            "AIModule",
            "NavigationSystem",
            "DialogSystemRuntime",
        });
	}
}
