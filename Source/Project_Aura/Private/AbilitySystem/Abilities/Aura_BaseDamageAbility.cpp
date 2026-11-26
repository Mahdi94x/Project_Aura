// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AbilitySystem/Abilities/Aura_BaseDamageAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAura_BaseDamageAbility::CauseDamage_Melee(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass,1.f);

	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
			*DamageSpecHandle.Data.Get(),
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UAura_BaseDamageAbility::MakeDamageEffectParamsFromAbilityDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAsc = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;
	
	return Params;
}

FTaggedMontage UAura_BaseDamageAbility::GetRandomTaggedMontageFromArray(
	const TArray<FTaggedMontage>& TaggedMontagesArray) const
{
	if (TaggedMontagesArray.Num() > 0)
	{
		const int32 RandomSelection = FMath::RandRange(0, TaggedMontagesArray.Num() - 1);
		return TaggedMontagesArray[RandomSelection];
	}
	return FTaggedMontage();
}
