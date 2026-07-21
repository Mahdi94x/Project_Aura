// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "Input/Aura_InputConfig.h"

const UInputAction* UAura_InputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FAuraInputActionStruct Action : this->AbilityInputActionsArray)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find InputAction for InputTag [%s] on AbilityInputAction [%s]"),
			*InputTag.ToString(),
			*GetNameSafe(this));
	}
	return nullptr;
}
