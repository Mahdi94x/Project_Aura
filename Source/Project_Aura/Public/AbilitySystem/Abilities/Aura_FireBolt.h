// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Aura_ProjectileSpellGA.h"
#include "Aura_FireBolt.generated.h"

UCLASS()
class PROJECT_AURA_API UAura_FireBolt : public UAura_ProjectileSpellGA
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
