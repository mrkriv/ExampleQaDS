using UnrealBuildTool;
using System.Collections.Generic;

public class DialogPluginExampleEditorTarget : TargetRules
{
	public DialogPluginExampleEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "DialogPluginExample" } );
	}
}
