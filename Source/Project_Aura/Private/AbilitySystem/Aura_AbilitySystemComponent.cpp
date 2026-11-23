// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "Aura_GameplayTags.h"
#include "AbilitySystem/Aura_AbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/Aura_BaseAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Interaction/PlayerInterface.h"
#include "Project_Aura/AuraLogChannels.h"

void UAura_AbilitySystemComponent::AbilityActorInfoSet()
{
	// bind callbacks to delegate after the InitAbilityActorInfo()
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAura_AbilitySystemComponent::Client_EffectApplied);
}

void UAura_AbilitySystemComponent::Client_EffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
												 const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}

void UAura_AbilitySystemComponent::GiveCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec =  FGameplayAbilitySpec(AbilityClass,1);
		
		if (const UAura_BaseAbility* AuraAbility = Cast<UAura_BaseAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraAbility->StartupInputTag);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FAura_GameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAura_AbilitySystemComponent::GiveCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
    	{
    		FGameplayAbilitySpec AbilitySpec =  FGameplayAbilitySpec(AbilityClass,1);
    		GiveAbilityAndActivateOnce(AbilitySpec);
    	}
}

void UAura_AbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAura_AbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAura_AbilitySystemComponent::ForEachAbility(const FForEachAbilitySignature& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in %hs"),__FUNCTION__);
		}
	}
}

FGameplayTag UAura_AbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (auto AssetTag : AbilitySpec.Ability.Get()->GetAssetTags())
		{
			if (AssetTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return AssetTag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAura_AbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (auto InputTag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (InputTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return InputTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAura_AbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (auto StatusTag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAura_AbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag InAssetTag)
{
	if (const FGameplayAbilitySpec* Spec = GetAbilitySpecFromAssetTag(InAssetTag))
	{
		return GetStatusTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UAura_AbilitySystemComponent::GetAbilitySpecFromAssetTag(const FGameplayTag& InAssetTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->GetAssetTags())
		{
			if (Tag.MatchesTag(InAssetTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

FGameplayTag UAura_AbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag InAssetTag)
{
	if (const FGameplayAbilitySpec* Spec = GetAbilitySpecFromAssetTag(InAssetTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

void UAura_AbilitySystemComponent::UpdateAbilityStatuses(int32 InLevel)
{
	UAbilityInfo* AbilityInfo = UAura_AbilitySystemLibrary::GetAbilityInfoDa(GetAvatarActor());
	
	for (const FAuraAbilityInfo& AbilityInfoStruct :AbilityInfo->AbilitiesInformation)
	{
		if (!AbilityInfoStruct.AssetTag.IsValid()) continue;
		if (InLevel < AbilityInfoStruct.LevelRequirement) continue;
		
		if (GetAbilitySpecFromAssetTag(AbilityInfoStruct.AssetTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityInfoStruct.Ability , 1);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FAura_GameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec); /*Force Replication down to clients*/
			Client_UpdateAbilityStatuses(AbilityInfoStruct.AssetTag,FAura_GameplayTags::Get().Abilities_Status_Eligible, AbilitySpec.Level);
		}
	}
}

void UAura_AbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetCharacterAttributePoints(GetAvatarActor()) > 0)
		{
			Server_UpgradeAttribute(AttributeTag);
		}
	}
}

void UAura_AbilitySystemComponent::Server_EquipAbility_Implementation(const FGameplayTag& AssetTag,
	const FGameplayTag& Slot)
{
	if (FGameplayAbilitySpec* Spec = GetAbilitySpecFromAssetTag(AssetTag))
	{
		const FAura_GameplayTags GameplayTags = FAura_GameplayTags::Get();
		
		const FGameplayTag& PrevSlot = GetInputTagFromSpec(*Spec);
		const FGameplayTag& Status = GetStatusTagFromSpec(*Spec);
		
		const bool bStatusValid = Status == GameplayTags.Abilities_Status_Equipped || Status == GameplayTags.Abilities_Status_Unlocked;
		
		if (bStatusValid)
		{
			// Remove this input tag from any ability that have it
			ClearAbilitiesOfSlot(Slot);
			
			// Clear this ability slot just in case it's a different slot
			ClearSlot(Spec);
			
			// now assign this ability to this slot
			Spec->GetDynamicSpecSourceTags().AddTag(Slot);
			
			if (Status == GameplayTags.Abilities_Status_Unlocked)
			{
				Spec->GetDynamicSpecSourceTags().RemoveTag(GameplayTags.Abilities_Status_Unlocked);
				Spec->GetDynamicSpecSourceTags().AddTag(GameplayTags.Abilities_Status_Equipped);
			}
			MarkAbilitySpecDirty(*Spec);
		}
		Client_EquipAbility(AssetTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
	}
}

void UAura_AbilitySystemComponent::Client_EquipAbility_Implementation(const FGameplayTag& AssetTag,
	const FGameplayTag& StatusTag, const FGameplayTag& Slot, const FGameplayTag& PrevSlot)
{
	AbilityEquippedDelegate.Broadcast(AssetTag, StatusTag, Slot, PrevSlot);
}

void UAura_AbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock ActiveScopesLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlotTag(&Spec, SlotTag))
		{
			ClearSlot(&Spec);
		}
	}
}

void UAura_AbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	if (Spec)
	{
		const FGameplayTag& Slot = GetInputTagFromSpec(*Spec);
		Spec->GetDynamicSpecSourceTags().RemoveTag(Slot);
		MarkAbilitySpecDirty(*Spec);
	}
}

bool UAura_AbilitySystemComponent::AbilityHasSlotTag(FGameplayAbilitySpec* Spec, const FGameplayTag& SlotTag)
{
	for (FGameplayTag Tag : Spec->GetDynamicSpecSourceTags())
	{
		if (Tag == SlotTag) return true;
	}
	return false;
}
bool UAura_AbilitySystemComponent::GetDescriptionsByAssetTag( const FGameplayTag& AssetTag, FString& OutDescription,
                                                              FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAssetTag(AssetTag))
	{
		if (UAura_BaseAbility* AuraAbility = Cast<UAura_BaseAbility>(AbilitySpec->Ability))
		{
			OutDescription = AuraAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = AuraAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}
	
	/* The ability is not activable -> Status = Locked*/
	const UAbilityInfo* AbilityInfo = UAura_AbilitySystemLibrary::GetAbilityInfoDa(GetAvatarActor());
	if (!AssetTag.IsValid() || AssetTag == FAura_GameplayTags::Get().Abilities_None)
	{
		OutDescription = FString();
	}
	else
	{
		OutDescription = UAura_BaseAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoByTag(AssetTag).LevelRequirement);
	}
	
	OutNextLevelDescription = FString();
	return false;
}

void UAura_AbilitySystemComponent::Server_SpendSpellPoints_Implementation(const FGameplayTag& AssetTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAssetTag(AssetTag))
	{
		const FAura_GameplayTags GameplayTags = FAura_GameplayTags::Get();
		FGameplayTag StatusTag = GetStatusTagFromSpec(*AbilitySpec);
		
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
		}
		
		if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
		{
			AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GameplayTags.Abilities_Status_Eligible);
			AbilitySpec->GetDynamicSpecSourceTags().AddTag(GameplayTags.Abilities_Status_Unlocked);
			StatusTag = GameplayTags.Abilities_Status_Unlocked;
		}
		else if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked) || StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
		{
			AbilitySpec->Level += 1;
		}
		
		Client_UpdateAbilityStatuses(AssetTag, StatusTag, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAura_AbilitySystemComponent::Server_UpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(),-1);
	}
}

void UAura_AbilitySystemComponent::Client_UpdateAbilityStatuses_Implementation(const FGameplayTag& AssetTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChangedDelegate.Broadcast(AssetTag, StatusTag, AbilityLevel);
}

void UAura_AbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}
