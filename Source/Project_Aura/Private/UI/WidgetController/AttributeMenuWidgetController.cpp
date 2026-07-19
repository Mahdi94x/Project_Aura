// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/Aura_AttributeSet.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAura_AttributeSet* AS = CastChecked<UAura_AttributeSet>(AttributeSet);
	
	for (auto& Pair : AS->TagsToAttributesMap)
	{
		BroadcastAttInfoToWidget(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
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
