// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "Interaction/HighlightInterface.h"
#include "AuraEnemy.generated.h"

UCLASS()
class PROJECT_AURA_API AAuraEnemy : public AAuraCharacterBase, public IHighlightInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();
	// ~ Begin IHighlight Interface
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;
	// ~ End IHighlight Interface

	// ~ Begin ICombat Interface
	virtual int32 GetCharacterLevel() override;
	// ~ End ICombat Interface

protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
	
};
