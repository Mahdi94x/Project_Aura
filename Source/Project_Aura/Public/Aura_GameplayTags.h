// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
/*
 * Aura_GameplayTags
 * Singleton containing native gameplay tags
 */
struct FAura_GameplayTags
{

	static const FAura_GameplayTags& Get() {return GameplayTags;}
	static void InitializeNativeGameplayTags();

	/*Primary Attributes*/
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	/*Secondary Attributes*/
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	
	/*Input Tags*/
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	
	/*DamageTypes*/
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Physical;
	
	/*Resistances (Extend to SecondaryAttributes)*/
	FGameplayTag Attributes_Secondary_FireResistance;
	FGameplayTag Attributes_Secondary_ArcaneResistance;
	FGameplayTag Attributes_Secondary_LightningResistance;
	FGameplayTag Attributes_Secondary_PhysicalResistance;
	
	/*Tags for Abilities*/
	FGameplayTag HitReact;
	
	/*DamageTypes Tags*/
	TMap<FGameplayTag, FGameplayTag> DamageToResistancesTagsMap;
	
private:
	static FAura_GameplayTags GameplayTags;
};
