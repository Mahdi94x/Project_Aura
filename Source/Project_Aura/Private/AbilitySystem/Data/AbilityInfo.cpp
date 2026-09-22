// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AbilitySystem/Data/AbilityInfo.h"

#include "Project_Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& AbilityInfo : AbilitiesInformation)
	{
		if (AbilityInfo.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return AbilityInfo;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, 
			TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	return FAuraAbilityInfo();
}
