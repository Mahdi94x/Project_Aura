// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(
	const ECharacterClass CharacterClass) 
{
	return CharacterClassDefaultInfoMap.FindChecked(CharacterClass);
}
