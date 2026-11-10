// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "AbilitySystem/Aura_AttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "PlayerState/Aura_PlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAuraAs()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAs()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAs()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAs()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetAuraPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetAuraPS()->OnLevelChangedDelegate.AddLambda([this](const int32 NewLevel)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
	});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAs()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAs()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAs()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAs()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	if (GetAuraAsc())
	{
		if (GetAuraAsc()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetAuraAsc()->AbilitiesGivenDelegate.AddUObject(this, &UAura_BaseWidgetController::BroadcastAbilityInfo);
		}
		
		GetAuraAsc()->EffectAssetTags.AddLambda(
	[this](const FGameplayTagContainer& AssetTagsContainer)
		{
			for (const FGameplayTag& Tag : AssetTagsContainer)
			{
				if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Message"))))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		});
	}
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	/*the xp amount received from playerState must be processed as controller needs to broadcast the xp progress bar percent*/
	
	const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo, please fill out AuraPlayerState Blueprint"));
	
	const int32 NextLevel = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();
	
	if (NextLevel<= MaxLevel && NextLevel > 0)
	{
		const int32 NextLevelXPRequirement = LevelUpInfo->LevelUpInformation[NextLevel].LevelUpRequirement;
		const int32 CurrentLevelXPRequirement = LevelUpInfo->LevelUpInformation[NextLevel - 1].LevelUpRequirement;
		
		const int32 XPRequiredForNextLevel = NextLevelXPRequirement - CurrentLevelXPRequirement;

		const int32 XPProgressWithinLevel = NewXP - CurrentLevelXPRequirement;

		const float XPBarPercent =
			static_cast<float>(XPProgressWithinLevel) /
			static_cast<float>(XPRequiredForNextLevel);
		
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}
