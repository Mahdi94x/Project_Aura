// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly) 
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(BlueprintReadOnly) /*Set from C++ side*/
	FGameplayTag InputTag = FGameplayTag();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<const UTexture2D> SpellIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<const UMaterialInterface> ImageBackground = nullptr;
	
};

UCLASS()
class PROJECT_AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FAuraAbilityInfo FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Aura|AbilityInformation")
	TArray<FAuraAbilityInfo> AbilitiesInformation;
};
