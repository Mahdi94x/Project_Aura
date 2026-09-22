// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Aura_AbilitySystemComponent.generated.h"

class UAura_AbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FStartupAbilitiesGivenSignature, UAura_AbilitySystemComponent* /*AuraAbilitySystemComponent*/);

UCLASS()
class PROJECT_AURA_API UAura_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/*function to be called at a certain point in time to bind the callback to the delegates
	 as the constructors are too early*/
	void AbilityActorInfoSet();
	
	void GiveCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	bool bStartupAbilitiesGiven = false;
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	FEffectAssetTags EffectAssetTags;
	FStartupAbilitiesGivenSignature AbilitiesGivenDelegate;
	
protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied /*The Callback Function*/
	(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};
