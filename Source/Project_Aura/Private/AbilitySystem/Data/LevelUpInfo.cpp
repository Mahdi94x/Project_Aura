// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 Xp) const
{
	int32 CurrentLevel = 1;
	bool bSearching = true;
	while (bSearching)
	{
		//LevelUpInformation[0] => Placeholder
		// LevelUpInformation[1] = Level 1 Information
		// LevelUpInformation[2] = Level 1 Information
		
		if (LevelUpInformation.Num() -1 <= CurrentLevel) return CurrentLevel; /*Boundary Check*/
		
		if (Xp >= LevelUpInformation[CurrentLevel].LevelUpRequirement) /*Main Condition*/
		{
			++CurrentLevel;
		}
		else
		{
			bSearching = false;
		}
	}
	return CurrentLevel;
}
