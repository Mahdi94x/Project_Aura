// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
/*
 * AuraGameplayTags
 * Singleton containing native gameplay tags
 */
struct FAuraGameplayTags
{
public:
static const FAuraGameplayTags& Get() {return GameplayTags;}
static void InitializeNativeGameplayTags();
protected:

private:
	static FAuraGameplayTags GameplayTags;
};
