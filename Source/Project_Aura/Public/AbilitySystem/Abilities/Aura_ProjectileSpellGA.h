// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Aura_BaseAbility.h"
#include "Aura_ProjectileSpellGA.generated.h"

class AAura_ProjectileActor;

UCLASS()
class PROJECT_AURA_API UAura_ProjectileSpellGA : public UAura_BaseAbility
{
	GENERATED_BODY()
	
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintCallable, Category="Aura|Projectile")
	void SpawnProjectile() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAura_ProjectileActor> ProjectileClass;
	
};
