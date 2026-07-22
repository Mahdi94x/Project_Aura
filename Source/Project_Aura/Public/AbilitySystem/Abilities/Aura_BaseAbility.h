// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Aura_BaseAbility.generated.h"

UCLASS()
class PROJECT_AURA_API UAura_BaseAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Aura|Input")
	FGameplayTag StartupInputTag;
	
};
