// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Aura_AssetManager.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_AURA_API UAura_AssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UAura_AssetManager& Get();
	virtual void StartInitialLoading() override;
};
