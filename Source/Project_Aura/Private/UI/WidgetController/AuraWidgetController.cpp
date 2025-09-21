// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	// WidgetController variable = the passed structure values
	this->PlayerController = WCParams.PlayerController;
	this->PlayerState = WCParams.PlayerState;
	this->AbilitySystemComponent = WCParams.AbilitySystemComponent;
	this->AttributeSet = WCParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
	
}
