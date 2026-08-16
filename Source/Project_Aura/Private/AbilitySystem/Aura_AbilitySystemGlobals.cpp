// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AbilitySystem/Aura_AbilitySystemGlobals.h"
#include "Aura_AbilityTypes.h"

FGameplayEffectContext* UAura_AbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAura_GameplayEffectContext();
}
