// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "Aura_AbilitySystemGlobals.generated.h"

UCLASS()
class PROJECT_AURA_API UAura_AbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
