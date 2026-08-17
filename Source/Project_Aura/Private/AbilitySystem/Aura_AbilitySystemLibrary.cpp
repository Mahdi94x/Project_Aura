// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AbilitySystem/Aura_AbilitySystemLibrary.h"

#include "Aura_AbilityTypes.h"
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
	UCharacterClassInfo* ClassInfoDa = GetCharacterClassInfoDa(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfoStruct =  ClassInfoDa->GetClassDefaultInfo(CharacterClass);
	const AActor* AvatarActor = Asc->GetAvatarActor(); /*MaxHealth - MaxMana Calculations*/
	
	FGameplayEffectContextHandle PrimaryContext = Asc->MakeEffectContext();
	PrimaryContext.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimarySpec = Asc->MakeOutgoingSpec(ClassDefaultInfoStruct.PrimaryAttributesEffect, Level, PrimaryContext);
	Asc->ApplyGameplayEffectSpecToSelf(*PrimarySpec.Data.Get());
	
	FGameplayEffectContextHandle SecondaryContext = Asc->MakeEffectContext();
	SecondaryContext.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondarySpec = Asc->MakeOutgoingSpec(ClassInfoDa->SecondaryAttributesEffect, Level, SecondaryContext);
	Asc->ApplyGameplayEffectSpecToSelf(*SecondarySpec.Data.Get());
	
	FGameplayEffectContextHandle VitalContext = Asc->MakeEffectContext();
	VitalContext.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalSpec = Asc->MakeOutgoingSpec(ClassInfoDa->VitalAttributesEffect, Level, VitalContext);
	Asc->ApplyGameplayEffectSpecToSelf(*VitalSpec.Data.Get());
	
}

void UAura_AbilitySystemLibrary::AddCharacterAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* Asc)
{
	if (UCharacterClassInfo* ClassInfoDa = GetCharacterClassInfoDa(WorldContextObject))
	{
		for (const TSubclassOf<UGameplayAbility> AbilityClass : ClassInfoDa->CommonAbilities)
		{
			FGameplayAbilitySpec AbilitySpec =  FGameplayAbilitySpec(AbilityClass,1);
			Asc->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAura_AbilitySystemLibrary::GetCharacterClassInfoDa(const UObject* WorldContextObject)
{
	const AAura_GameModeBase* AuraGameMode = Cast<AAura_GameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	return AuraGameMode->CharacterClassInfo;
}

bool UAura_AbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAura_GameplayEffectContext* AuraContext = static_cast<const FAura_GameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetBlockedHit();
	}
	return false;
}

bool UAura_AbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAura_GameplayEffectContext* AuraContext = static_cast<const FAura_GameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetCriticalHit();
	}
	return false;
}

void UAura_AbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockedHit)
{
	if (FAura_GameplayEffectContext* AuraContext = static_cast<FAura_GameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->SetBlockedHit(bIsBlockedHit);
	}
}

void UAura_AbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bIsCriticalHit)
{
	if (FAura_GameplayEffectContext* AuraContext = static_cast<FAura_GameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->SetCriticalHit(bIsCriticalHit);
	}
}


