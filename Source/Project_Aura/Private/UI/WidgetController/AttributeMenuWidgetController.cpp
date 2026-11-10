// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "AbilitySystem/Aura_AttributeSet.h"
#include "PlayerState/Aura_PlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	for (auto& Pair : GetAuraAs()->TagsToAttributesMap)
	{
		BroadcastAttInfoToWidget(Pair.Key, Pair.Value());
	}
	
	OnPlayerAttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda([this](int32 NewAttributePoints)
	{
		OnPlayerAttributePointsChangedDelegate.Broadcast(NewAttributePoints);
	});
	
	check(AttributeInfoDataAsset);
	for (auto& Pair : GetAuraAs()->TagsToAttributesMap)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttInfoToWidget(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuraAsc()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttInfoToWidget(const FGameplayTag& GameplayTag,
                                                              const FGameplayAttribute& GameplayAttribute) const
{
	checkf(AttributeInfoDataAsset, TEXT("Set the Data Asset in BP_AttributeMenuWidgetController"));
	FAuraAttributeInfo Info = AttributeInfoDataAsset->FindAttributeInfoByTag(GameplayTag);
	Info.AttributeValue = GameplayAttribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
