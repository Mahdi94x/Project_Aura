// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Aura_BaseAbility.h"
#include "Aura_SummonAbility.generated.h"

UCLASS()
class PROJECT_AURA_API UAura_SummonAbility : public UAura_BaseAbility
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();
	
	UPROPERTY(EditDefaultsOnly, Category = "Aura|Summoning")
	int32 NumOfMinions = 5;
	
	UPROPERTY(EditDefaultsOnly, Category = "Aura|Summoning")
	TArray<TSubclassOf<APawn>> MinionsClasses;
	
	UPROPERTY(EditDefaultsOnly, Category = "Aura|Summoning")
	float MinSpawnDistance = 150.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Aura|Summoning")
	float MaxSpawnDistance = 400.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Aura|Summoning")
	float SpawnSpreadAngle = 120.f;
};
