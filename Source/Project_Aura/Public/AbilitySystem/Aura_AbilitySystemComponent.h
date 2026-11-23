// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Aura_AbilitySystemComponent.generated.h"

class UAura_AbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FStartupAbilitiesGivenSignature);
DECLARE_DELEGATE_OneParam(FForEachAbilitySignature, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChangedSignature, const FGameplayTag& /*AssetTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquippedSignature, const FGameplayTag& /*AssetTags*/, const FGameplayTag& /*Status*/, const FGameplayTag& /*Slot*/ , const FGameplayTag& /*PrevSlot*/)

UCLASS()
class PROJECT_AURA_API UAura_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/*function to be called at a certain point in time to bind the callback to the delegates
	 as the constructors are too early*/
	void AbilityActorInfoSet();
	
	void GiveCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void GiveCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbilitySignature& Delegate);
	
	/*Utility func to return ability tag from ability spec*/
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	/*Utility func to return input tag from ability spec*/
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	/*Utility func to return status tag from ability spec*/
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	/*Utility func to return status tag from ability Tag, can't be static, using one of the member variables*/
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag InAssetTag);
	
	/*Utility func to return status tag from ability Tag, can't be static, using one of the member variables*/
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag InAssetTag);
	
	/*Utility function to return the already activated ability spec from the asset tag, if the ability is not active it returns a nullptr*/
	FGameplayAbilitySpec* GetAbilitySpecFromAssetTag(const FGameplayTag& InAssetTag);
	
	void UpdateAbilityStatuses(int32 InLevel);
	
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	
	UFUNCTION(Server, Reliable)
	void Server_UpgradeAttribute(const FGameplayTag& AttributeTag);
	
	UFUNCTION(Server, Reliable)
	void Server_SpendSpellPoints(const FGameplayTag& AssetTag);
	
	UFUNCTION(Server, Reliable)
	void Server_EquipAbility(const FGameplayTag& AssetTag, const FGameplayTag& Slot);
	
	UFUNCTION(Client, Reliable)
	void Client_EquipAbility(const FGameplayTag& AssetTag, const FGameplayTag& StatusTag, const FGameplayTag& Slot, const FGameplayTag& PrevSlot);
	
	bool GetDescriptionsByAssetTag(const FGameplayTag& AssetTag, FString& OutDescription, FString& OutNextLevelDescription);
	
	void ClearSlot(FGameplayAbilitySpec* Spec);
	void ClearAbilitiesOfSlot(const FGameplayTag& SlotTag);
	static bool AbilityHasSlotTag(FGameplayAbilitySpec* Spec, const FGameplayTag& SlotTag);
	
	FEffectAssetTags EffectAssetTags;
	FStartupAbilitiesGivenSignature AbilitiesGivenDelegate;
	FAbilityStatusChangedSignature AbilityStatusChangedDelegate;
	FAbilityEquippedSignature AbilityEquippedDelegate;
	bool bStartupAbilitiesGiven = false;
	
protected:
	
	UFUNCTION(Client, Reliable)
	void Client_EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
	UFUNCTION(Client, Reliable)
	void Client_UpdateAbilityStatuses(const FGameplayTag& AssetTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
	
	virtual void OnRep_ActivateAbilities() override;
	
};
