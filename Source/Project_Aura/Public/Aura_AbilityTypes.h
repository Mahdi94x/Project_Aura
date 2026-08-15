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
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
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
