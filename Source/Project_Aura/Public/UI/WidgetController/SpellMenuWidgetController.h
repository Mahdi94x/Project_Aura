// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Aura_BaseWidgetController.h"
#include "Aura_GameplayTags.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"

struct FAura_GameplayTags;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool,
                                             bEquipButtonEnabled);

struct FSelectedAbility
{
	FGameplayTag AbilityAssetTag = FGameplayTag();
	FGameplayTag AbilityStatusTag = FGameplayTag();
};

UCLASS(BlueprintType, Blueprintable)
class PROJECT_AURA_API USpellMenuWidgetController : public UAura_BaseWidgetController
{
	GENERATED_BODY()
	
public:
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AssetTag);
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|SpellPoints")
	FOnPlayerInfoChangedSignature OnPlayerSpellPointsChangedDelegate;
	
private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButtons);
	FSelectedAbility SelectedAbility = FSelectedAbility(FAura_GameplayTags::Get().Abilities_None, FAura_GameplayTags::Get().Abilities_Status_Locked);
	int32 CurrentSpellPoints = 0;
};
