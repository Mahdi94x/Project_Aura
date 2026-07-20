// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "Aura_BaseCharacter.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class PROJECT_AURA_API AAura_BaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAura_BaseCharacter();
	virtual int32 GetCharacterLevel() override;
	virtual void BeginPlay() override;
	
protected:
	virtual void InitializeAbilityActorInfo();
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	void InitializeDefaultAttributes() const;
	virtual void AddCharacterAbilities();
	
	UPROPERTY(EditAnywhere, Category = "Aura|Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Aura|Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Aura|Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Aura|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

public: /*Setters and Getters*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;
	
private:
	UPROPERTY(EditAnywhere, Category = "Aura|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilitiesArray;
	
};
