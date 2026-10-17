// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Aura_BaseCharacter.h"
#include "Interaction/PlayerInterface.h"
#include "Aura_PlayerCharacter.generated.h"

UCLASS()
class PROJECT_AURA_API AAura_PlayerCharacter : public AAura_BaseCharacter, public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAura_PlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	// ~ Begin ICombat Interface
	virtual int32 GetCharacterLevel() override;
	// ~ End ICombat Interface
	
	// ~ Begin IPlayer Interface
	virtual void AddtoXP_Implementation(int32 InXP) override;
	// ~ End IPlayer Interface
	
protected: 
	virtual void InitializeAbilityActorInfo() override;

};
