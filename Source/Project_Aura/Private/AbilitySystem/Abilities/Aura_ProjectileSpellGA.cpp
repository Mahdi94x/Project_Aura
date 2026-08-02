// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "AbilitySystem/Abilities/Aura_ProjectileSpellGA.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/Aura_ProjectileActor.h"
#include "Interaction/CombatInterface.h"


void UAura_ProjectileSpellGA::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, 
                                              const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UAura_ProjectileSpellGA::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	checkf(ProjectileClass, TEXT("Set the ProjectileClass in the GameplayAbility Details Panel"));
	if (const TScriptInterface<ICombatInterface> CombatInterface = GetAvatarActorFromActorInfo())
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(CombatInterface->GetCombatSocketLocation());
		FRotator Rotation = (ProjectileTargetLocation - CombatInterface->GetCombatSocketLocation()).Rotation();
		Rotation.Pitch = 0.f;
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		AAura_ProjectileActor* Projectile = GetWorld()->SpawnActorDeferred<AAura_ProjectileActor>
		(ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
		
		checkf(DamageEffectClass, TEXT("Set the DamageEffectClass in the GameplayAbility Details Panel"));
		const UAbilitySystemComponent* SourceAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		Projectile->DamageEffectSpecHandle = SourceAsc->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceAsc->MakeEffectContext());
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
