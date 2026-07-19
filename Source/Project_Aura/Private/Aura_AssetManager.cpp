// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "Aura_AssetManager.h"
#include "Aura_GameplayTags.h"

UAura_AssetManager& UAura_AssetManager::Get()
{
	check(GEngine);
	UAura_AssetManager* AuraAssetManager = Cast<UAura_AssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAura_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FAura_GameplayTags::InitializeNativeGameplayTags();
}
