// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 Xp) const
{
	/*this algorithm returns the level based on the next level threshold,
	 * so if the player is currently in level 2 moving to level 3,
	 * the algorithm will return level 3 as the current xp is higher than
	 * the threshold for level 2 but lower than the threshold for level 3
	 * //LevelUpInformation[0] => Placeholder
	 */
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		if (LevelUpInformation.Num() -1 <= Level) return Level; /*Boundary Check*/
		
		if (Xp >= LevelUpInformation[Level].LevelUpRequirement) /*Main Condition*/
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
