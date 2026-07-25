// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "AbilitySystem/Abilities/Aura_ProjectileSpell.h"
#include "Kismet/KismetSystemLibrary.h"


void UAura_ProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UKismetSystemLibrary::PrintString(this,
		FString("ActivateAbility::Aura_ProjectileSpell(C++)"),true, true, FColor::Yellow, 3.f);
}
