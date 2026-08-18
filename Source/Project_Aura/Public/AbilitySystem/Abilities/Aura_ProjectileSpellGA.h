// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Aura_BaseDamageAbility.h"
#include "Aura_ProjectileSpellGA.generated.h"

class AAura_ProjectileActor;
class UGameplayEffect;

UCLASS()
class PROJECT_AURA_API UAura_ProjectileSpellGA : public UAura_BaseDamageAbility
{
	GENERATED_BODY()
	
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintCallable, Category = "Aura|ProjectileSpell")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|ProjectileSpell")
	TSubclassOf<AAura_ProjectileActor> ProjectileClass;
	

	
};
