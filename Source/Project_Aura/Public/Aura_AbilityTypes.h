#pragma once

#include "GameplayEffectTypes.h"
#include "Aura_AbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()
	
	FDamageEffectParams(){}
	
	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;
	
	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAsc = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAsc = nullptr;
	
	UPROPERTY()
	float BaseDamage = 0.f;
	
	UPROPERTY()
	float AbilityLevel = 1.f;
	
	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY()
	float DebuffChance = 0.f;
	
	UPROPERTY()
	float DebuffDamage = 0.f;
	
	UPROPERTY()
	float DebuffDuration = 0.f;
	
	UPROPERTY()
	float DebuffFrequency = 0.f;
};

USTRUCT(BlueprintType)
struct FAura_GameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
	/** Returns the actual struct used for serialization. Subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override;
	
	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
	
	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAura_GameplayEffectContext* Duplicate() const override;
	
	bool GetBlockedHit() const {return bIsBlockedHit;}
	bool GetCriticalHit() const {return bIsCriticalHit;}
	void SetBlockedHit(bool bNewBlockedHit) {bIsBlockedHit = bNewBlockedHit;}
	void SetCriticalHit(bool bNewCriticalHit) {bIsCriticalHit = bNewCriticalHit;}
	
protected:
	
	UPROPERTY()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;
	
};

template<>
struct TStructOpsTypeTraits< FAura_GameplayEffectContext > : TStructOpsTypeTraitsBase2< FAura_GameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
