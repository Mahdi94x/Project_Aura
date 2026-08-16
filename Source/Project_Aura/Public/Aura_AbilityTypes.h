#pragma once

#include "GameplayEffectTypes.h"
#include "Aura_AbilityTypes.generated.h"

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
