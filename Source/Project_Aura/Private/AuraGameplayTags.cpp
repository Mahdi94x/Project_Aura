// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags; // creating the single instance

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	/*
	 *Primary Attributes
	 */
	GameplayTags.Attributes_Primary_Strength =
	    TagsManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases physical damage.")
	);

	GameplayTags.Attributes_Primary_Intelligence =
		TagsManager.AddNativeGameplayTag(
			FName("Attributes.Primary.Intelligence"),
			FString("Increases magical damage.")
		);

	GameplayTags.Attributes_Primary_Resilience =
		TagsManager.AddNativeGameplayTag(
			FName("Attributes.Primary.Resilience"),
			FString("Increases armor and armor penetration.")
		);

	GameplayTags.Attributes_Primary_Vigor =
		TagsManager.AddNativeGameplayTag(
			FName("Attributes.Primary.Vigor"),
			FString("Increases health.")
		);

	/*
	 *Secondary Attributes
	 */
	GameplayTags.Attributes_Secondary_Armor =
        TagsManager.AddNativeGameplayTag(
            FName("Attributes.Secondary.Armor"),
            FString("Reduces damage taken and improves block chance.")
    );

    GameplayTags.Attributes_Secondary_ArmorPenetration =
        TagsManager.AddNativeGameplayTag(
            FName("Attributes.Secondary.ArmorPenetration"),
            FString("Ignores a percentage of enemy armor.")
        );

    GameplayTags.Attributes_Secondary_BlockChance =
        TagsManager.AddNativeGameplayTag(
            FName("Attributes.Secondary.BlockChance"),
            FString("Chance to block incoming attacks.")
        );

    GameplayTags.Attributes_Secondary_CriticalHitChance =
        TagsManager.AddNativeGameplayTag(
            FName("Attributes.Secondary.CriticalHitChance"),
            FString("Chance to double damage plus critical hit bonus")
        );

    GameplayTags.Attributes_Secondary_CriticalHitDamage =
        TagsManager.AddNativeGameplayTag(
            FName("Attributes.Secondary.CriticalHitDamage"),
            FString("Bonus damage added when a critical hit is scored")
        );

    GameplayTags.Attributes_Secondary_CriticalHitResistance =
        TagsManager.AddNativeGameplayTag(
            FName("Attributes.Secondary.CriticalHitResistance"),
            FString("Reduces critical hit chance of attacking enemies")
        );

    GameplayTags.Attributes_Secondary_HealthRegeneration =
        TagsManager.AddNativeGameplayTag(
            FName("Attributes.Secondary.HealthRegeneration"),
            FString("Amount of health regenerated every 1 second")
        );

    GameplayTags.Attributes_Secondary_ManaRegeneration =
        TagsManager.AddNativeGameplayTag(
            FName("Attributes.Secondary.ManaRegeneration"),
            FString("Amount of mana regenerated every 1 second")
        );

    GameplayTags.Attributes_Secondary_MaxHealth =
        TagsManager.AddNativeGameplayTag(
            FName("Attributes.Secondary.MaxHealth"),
            FString("Maximum amount of health obtainable")
        );

    GameplayTags.Attributes_Secondary_MaxMana =
        TagsManager.AddNativeGameplayTag(
            FName("Attributes.Secondary.MaxMana"),
            FString("Maximum amount of mana obtainable")
        );
}
