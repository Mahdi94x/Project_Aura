// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "UI/WidgetController/Aura_BaseWidgetController.h"
#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "AbilitySystem/Aura_AttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "PlayerController/Aura_PlayerController.h"
#include "PlayerState/Aura_PlayerState.h"

void UAura_BaseWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	// WidgetController variable = the passed structure values
	this->PlayerController = WCParams.PlayerController;
	this->PlayerState = WCParams.PlayerState;
	this->AbilitySystemComponent = WCParams.AbilitySystemComponent;
	this->AttributeSet = WCParams.AttributeSet;
}

void UAura_BaseWidgetController::BroadcastInitialValues()
{
	
}

void UAura_BaseWidgetController::BindCallbacksToDependencies()
{
	
}

void UAura_BaseWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuraAsc()->bStartupAbilitiesGiven) return;
	
	/*Create ability struct that the widgets need to show the icon and the background at the corresponding input globe*/
	FForEachAbilitySignature BroadcastDelegate; /*Declared in Aura_AbilitySystemComponent*/
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = AuraAbilitySystemComponent->GetStatusTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info); /*for WBP_SpellGlobeBase Widget and WBP_EquippedRow_Button*/
	});
	
	/*pass the delegate to the AuraAsc so that it execute it for every ability in the startup array*/
	GetAuraAsc()->ForEachAbility(BroadcastDelegate);
}

AAura_PlayerController* UAura_BaseWidgetController::GetAuraPC()
{
	if (AuraPlayerController == nullptr)
	{
		AuraPlayerController = Cast<AAura_PlayerController>(this->PlayerController);
	}
	return AuraPlayerController;
}

AAura_PlayerState* UAura_BaseWidgetController::GetAuraPS()
{
	if (AuraPlayerState == nullptr)
	{
		AuraPlayerState = Cast<AAura_PlayerState>(this->PlayerState);
	}
	return AuraPlayerState;
}

UAura_AbilitySystemComponent* UAura_BaseWidgetController::GetAuraAsc()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAura_AbilitySystemComponent>(this->AbilitySystemComponent);
	}
	return AuraAbilitySystemComponent;
}

UAura_AttributeSet* UAura_BaseWidgetController::GetAuraAs()
{
	if (AuraAttributeSet == nullptr)
	{
		AuraAttributeSet = Cast<UAura_AttributeSet>(this->AttributeSet);
	}
	return AuraAttributeSet;
}
