// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "Aura_GameplayTags.h"
#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "PlayerState/Aura_PlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	OnPlayerSpellPointsChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	/*Ability Status change from Aura_AbilitySystemComponent*/
	GetAuraAsc()->AbilityStatusChangedDelegate.AddLambda
	([this] (const FGameplayTag& AssetTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
	{
		if (SelectedAbility.AbilityAssetTag == AssetTag)
		{
			SelectedAbility.AbilityStatusTag = StatusTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints,bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			GetAuraAsc()->GetDescriptionsByAssetTag(AssetTag,Description, NextLevelDescription);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
		}
		
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AssetTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
	
	/*Spell Point Change from Aura_PlayerState*/
	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 NewValue)
	{
		OnPlayerSpellPointsChangedDelegate.Broadcast(NewValue);
		CurrentSpellPoints = NewValue;
		
		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		FString Description;
		FString NextLevelDescription;
		GetAuraAsc()->GetDescriptionsByAssetTag(SelectedAbility.AbilityAssetTag,Description, NextLevelDescription);
		ShouldEnableButtons(SelectedAbility.AbilityStatusTag, CurrentSpellPoints, bEnableSpendPoints,bEnableEquip);
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
	});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AssetTag)
{
	const FAura_GameplayTags GameplayTags = FAura_GameplayTags::Get();
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	
	FGameplayTag AbilityStatus;
	
	const bool bTagValid = AssetTag.IsValid();
	const bool bTagNone = AssetTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraAsc()->GetAbilitySpecFromAssetTag(AssetTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraAsc()->GetStatusTagFromSpec(*AbilitySpec);
	}
	
	SelectedAbility.AbilityAssetTag = AssetTag;
	SelectedAbility.AbilityStatusTag = AbilityStatus;
	
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	FString Description;
	FString NextLevelDescription;
	GetAuraAsc()->GetDescriptionsByAssetTag(AssetTag,Description, NextLevelDescription);
	ShouldEnableButtons(SelectedAbility.AbilityStatusTag, SpellPoints, bEnableSpendPoints,bEnableEquip);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointsButtonPressed()
{
	if (GetAuraAsc())
	{
		GetAuraAsc()->Server_SpendSpellPoints(SelectedAbility.AbilityAssetTag);
	}
}

void USpellMenuWidgetController::GlobeDeselected()
{
	SelectedAbility.AbilityAssetTag = FAura_GameplayTags::Get().Abilities_None;
	SelectedAbility.AbilityStatusTag = FAura_GameplayTags::Get().Abilities_Status_Locked;
	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
                                                     bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButtons)
{
	const FAura_GameplayTags GameplayTags = FAura_GameplayTags::Get();
	
	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButtons = false;
	
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButtons = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButtons = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
}
