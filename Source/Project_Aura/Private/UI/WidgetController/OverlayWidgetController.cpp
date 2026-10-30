// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "AbilitySystem/Aura_AttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "PlayerState/Aura_PlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAura_AttributeSet* AuraAttributeSet = CastChecked<UAura_AttributeSet>(this->AttributeSet);
	
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AAura_PlayerState* AuraPlayerState = CastChecked<AAura_PlayerState>(this->PlayerState);
	AuraPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	AuraPlayerState->OnLevelChangedDelegate.AddLambda([this](const int32 NewLevel)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
	});
	
	const UAura_AttributeSet* AuraAttributeSet = CastChecked<UAura_AttributeSet>(this->AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	if (UAura_AbilitySystemComponent* AuraAsc = Cast<UAura_AbilitySystemComponent>(this->AbilitySystemComponent))
	{
		if (AuraAsc->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(AuraAsc);
		}
		else
		{
			AuraAsc->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}
		
		AuraAsc->EffectAssetTags.AddLambda(
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

void UOverlayWidgetController::OnInitializeStartupAbilities(UAura_AbilitySystemComponent* AuraAbilitySystemComponent)
{
	//TODO: get information about all given abilities, look up their ability info and broadcast it to the widget
	
	if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven) return;
	
	FForEachAbilitySignature BroadcastDelegate; /*Declared in Aura_AbilitySystemComponent*/
	
	BroadcastDelegate.BindLambda([this, AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		// TODO: Find a way to figure out the ability tag for a given ability spec-> UAura_AbilitySystemComponent::GetAbilityTagFromSpec
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		
		// TODO: getting the input tag before broadcasting
		Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		
		AbilityInfoDelegate.Broadcast(Info);
	});
	
	AuraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	/*the xp amount received from playerState must be processed as controller needs to broadcast the xp progress bar percent*/
	
	const AAura_PlayerState* AuraPlayerState = CastChecked<AAura_PlayerState>(this->PlayerState);
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
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
