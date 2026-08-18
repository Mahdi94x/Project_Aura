// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Aura_BaseAbility.h"
#include "Aura_BaseDamageAbility.generated.h"

UCLASS()
class PROJECT_AURA_API UAura_BaseDamageAbility : public UAura_BaseAbility
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Aura|Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypesMap;
};
