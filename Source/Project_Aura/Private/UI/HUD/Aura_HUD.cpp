// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "UI/HUD/Aura_HUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/Aura_UserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAura_HUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies(); // bind callbacks after the construction of the controller
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAura_HUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

void AAura_HUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// checks
	checkf(OverlayWidgetClass, TEXT("Overlay widget class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay widget controller class uninitialized, please fill out BP_AuraHUD"));

	// Constructing the widget
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAura_UserWidget>(Widget); // setting the member variable of the AuraHUD
	
	// Constructing the widget controller if it does not exist
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	// set the widget controller for the overlay
	OverlayWidget->SetWidgetController(WidgetController);

	// broadcasting the initial values through the widget controller
	WidgetController->BroadcastInitialValues();
	
	// display the widget
	Widget->AddToViewport();
}
