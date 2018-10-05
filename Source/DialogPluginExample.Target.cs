

using UnrealBuildTool;
using System.Collections.Generic;

public class DialogPluginExampleTarget : TargetRules
{
	public DialogPluginExampleTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "DialogPluginExample" } );
	}
}
