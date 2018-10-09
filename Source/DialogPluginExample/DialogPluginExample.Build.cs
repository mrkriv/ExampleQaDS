

using UnrealBuildTool;

public class DialogPluginExample : ModuleRules
{
	public DialogPluginExample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
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
			"ShaderCore",

			"NavigationSystem"
		});
	}
}
