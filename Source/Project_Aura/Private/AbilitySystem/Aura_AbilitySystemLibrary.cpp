// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AbilitySystem/Aura_AbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/Aura_PlayerState.h"
#include "UI/HUD/Aura_HUD.h"
#include "UI/WidgetController/Aura_BaseWidgetController.h"

UOverlayWidgetController* UAura_AbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAura_HUD* AuraHUD = Cast<AAura_HUD>(PC->GetHUD()))
		{
			AAura_PlayerState* PS = PC->GetPlayerState<AAura_PlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAura_AbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAura_HUD* AuraHUD = Cast<AAura_HUD>(PC->GetHUD()))
		{
			AAura_PlayerState* PS = PC->GetPlayerState<AAura_PlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}
