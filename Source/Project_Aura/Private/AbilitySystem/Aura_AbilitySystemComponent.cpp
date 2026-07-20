// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "Aura_GameplayTags.h"

void UAura_AbilitySystemComponent::AbilityActorInfoSet()
{
	// bind callbacks to delegate after the InitAbilityActorInfo()
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAura_AbilitySystemComponent::EffectApplied);
}

void UAura_AbilitySystemComponent::GrantCharacterAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (auto AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec =  FGameplayAbilitySpec(AbilityClass,1);
		//GiveAbility(AbilitySpec);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAura_AbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                 const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
