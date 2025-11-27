// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

using UnrealBuildTool;
using System.Collections.Generic;

public class Project_AuraEditorTarget : TargetRules
{
	public Project_AuraEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;

        BuildEnvironment = TargetBuildEnvironment.Shared;

        bOverrideBuildEnvironment = true;

        ExtraModuleNames.AddRange( new string[] { "Project_Aura" } );
	}
}
