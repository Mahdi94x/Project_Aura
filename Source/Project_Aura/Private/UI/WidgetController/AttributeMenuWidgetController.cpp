// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/Aura_AttributeSet.h"
#include "PlayerState/Aura_PlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAura_AttributeSet* AS = CastChecked<UAura_AttributeSet>(AttributeSet);
	
	for (auto& Pair : AS->TagsToAttributesMap)
	{
		BroadcastAttInfoToWidget(Pair.Key, Pair.Value());
	}
	
	const AAura_PlayerState* AuraPlayerState = CastChecked<AAura_PlayerState>(this->PlayerState);
	OnPlayerAttributePointsChangedDelegate.Broadcast(AuraPlayerState->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	AAura_PlayerState* AuraPlayerState = CastChecked<AAura_PlayerState>(this->PlayerState);
	AuraPlayerState->OnAttributePointsChangedDelegate.AddLambda([this](int32 NewAttributePoints)
	{
		OnPlayerAttributePointsChangedDelegate.Broadcast(NewAttributePoints);
	});
	
	UAura_AttributeSet* AS = CastChecked<UAura_AttributeSet>(AttributeSet);
	
	for (auto& Pair : AS->TagsToAttributesMap)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttInfoToWidget(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastAttInfoToWidget(const FGameplayTag& GameplayTag,
	const FGameplayAttribute& GameplayAttribute) const
{
	checkf(AttributeInfoDataAsset, TEXT("Set the Data Asset in BP_AttributeMenuWidgetController"));
	FAuraAttributeInfo Info = AttributeInfoDataAsset->FindAttributeInfoByTag(GameplayTag);
	Info.AttributeValue = GameplayAttribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
