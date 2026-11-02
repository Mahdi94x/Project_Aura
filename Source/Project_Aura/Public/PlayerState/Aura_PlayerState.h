// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Aura_PlayerState.generated.h"

class ULevelUpInfo;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32 /*StatValue*/);

UCLASS()
class PROJECT_AURA_API AAura_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AAura_PlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	FOnPlayerStatChangedSignature OnXPChangedDelegate;
	FOnPlayerStatChangedSignature OnLevelChangedDelegate;
	FOnPlayerStatChangedSignature OnAttributePointsChangedDelegate;
	FOnPlayerStatChangedSignature OnSpellPointsChangedDelegate;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	UFUNCTION()
	void OnRep_XP(int32 OldXP);
	
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing= OnRep_Level)
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing= OnRep_XP)
	int32 XP = 1;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing= OnRep_AttributePoints)
	int32 AttributePoints = 0;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing= OnRep_SpellPoints)
	int32 SpellPoints = 0;

public: /*Setters and Getters*/
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;
	
	FORCEINLINE int32 GetPlayerStateLevel() const {return this->Level;}
	FORCEINLINE int32 GetXP() const {return this->XP;}
	FORCEINLINE int32 GetAttributePoints() const {return this->AttributePoints;}
	FORCEINLINE int32 GetSpellPoints() const {return this->SpellPoints;}
	
	void SetXP(int32 NewXP);
	void SetLevel(int32 NewLevel);
	
	void AddToXP(int32 NewXP);
	void AddToLevel(int32 NewLevel);
	void AddToAttributePoints(int32 NewAttributePoints);
	void AddToSpellPoints(int32 NewSpellPoints);
};
