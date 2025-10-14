// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& InfoStruct : AttributeInformationArray)
	{
		if (InfoStruct.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return InfoStruct;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]"),
			*AttributeTag.ToString(),
			*GetNameSafe(this));
	}
	return FAuraAttributeInfo(); // return an empty struct
}
