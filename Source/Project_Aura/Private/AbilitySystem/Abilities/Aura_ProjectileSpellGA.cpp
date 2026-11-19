// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "AbilitySystem/Abilities/Aura_ProjectileSpellGA.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura_GameplayTags.h"
#include "Actor/Aura_ProjectileActor.h"
#include "Interaction/CombatInterface.h"


FString UAura_ProjectileSpellGA::GetDescription(int32 Level)
{
	const int32 Damage = DamageTypesMap[FAura_GameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			"<Default>Launches a bolt of fire, exploding on impact and dealing: </>"
			"<Damage>%d</>"
			"<Default> fire damage with a chance to burn</>\n\n"
			"<Small>Level: </>"
			"<Level>%d</>"),
			Damage, Level);
	}
	
	return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			"<Default>Launches %d bolts of fire, exploding on impact and dealing: </>"
			"<Damage>%d</>"
			"<Default> fire damage with a chance to burn</>\n\n"
			"<Small>Level: </>"
			"<Level>%d</>"),
			FMath::Min(Level,NumProjectiles),Damage, Level);
}

FString UAura_ProjectileSpellGA::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = DamageTypesMap[FAura_GameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL: </>\n\n"
		"<Default>Launches %d bolts of fire, exploding on impact and dealing: </>"
		"<Damage>%d</><Default> fire damage with a chance to burn</>\n\n"
		"<Small>Level: </>"
		"<Level>%d</>"),
		FMath::Min(Level, NumProjectiles), Damage, Level);
}

void UAura_ProjectileSpellGA::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
       const FGameplayAbilityActorInfo* ActorInfo, 
       const FGameplayAbilityActivationInfo ActivationInfo,
       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UAura_ProjectileSpellGA::SpawnProjectile(const FVector& ProjectileTargetLocation , const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	checkf(ProjectileClass, TEXT("Set the ProjectileClass in the GameplayAbility Details Panel"));
	
	FTransform SpawnTransform;
	const FVector Location = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	SpawnTransform.SetLocation(Location);
	FRotator Rotation = (ProjectileTargetLocation - Location).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	SpawnTransform.SetRotation(Rotation.Quaternion());
		
	AAura_ProjectileActor* Projectile = GetWorld()->SpawnActorDeferred<AAura_ProjectileActor>
	(ProjectileClass,
	SpawnTransform,
	GetOwningActorFromActorInfo(),
	Cast<APawn>(GetOwningActorFromActorInfo()),
	ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
		
	Projectile->SetInstigator(Cast<APawn>(GetAvatarActorFromActorInfo()));
		
	const UAbilitySystemComponent* SourceAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceAsc->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);
	const FGameplayEffectSpecHandle SpecHandle = SourceAsc->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
		
	for (auto& Pair : DamageTypesMap)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}
		
	Projectile->DamageEffectSpecHandle = SpecHandle;
		
	Projectile->FinishSpawning(SpawnTransform);
}
