// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AbilitySystem/Aura_AbilitySystemLibrary.h"

#include "GameMode/Aura_GameModeBase.h"
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

void UAura_AbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* Asc)
{
	const AAura_GameModeBase* AuraGameMode = Cast<AAura_GameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return;
	
	UCharacterClassInfo* ClassInfoDa = AuraGameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo ClassDefaultInfoStruct =  ClassInfoDa->GetClassDefaultInfo(CharacterClass);
	const AActor* AvatarActor = Asc->GetAvatarActor(); /*MaxHealth - MaxMana Calculations*/
	
	FGameplayEffectContextHandle PrimaryContext = Asc->MakeEffectContext();
	PrimaryContext.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimarySpec = Asc->MakeOutgoingSpec(ClassDefaultInfoStruct.PrimaryAttributes, Level, PrimaryContext);
	Asc->ApplyGameplayEffectSpecToSelf(*PrimarySpec.Data.Get());
	
	FGameplayEffectContextHandle SecondaryContext = Asc->MakeEffectContext();
	SecondaryContext.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondarySpec = Asc->MakeOutgoingSpec(ClassInfoDa->SecondaryAttributes, Level, SecondaryContext);
	Asc->ApplyGameplayEffectSpecToSelf(*SecondarySpec.Data.Get());
	
	FGameplayEffectContextHandle VitalContext = Asc->MakeEffectContext();
	VitalContext.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalSpec = Asc->MakeOutgoingSpec(ClassInfoDa->VitalAttributes, Level, VitalContext);
	Asc->ApplyGameplayEffectSpecToSelf(*VitalSpec.Data.Get());
	
}
