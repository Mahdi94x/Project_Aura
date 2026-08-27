// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enemy_HighlightInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemy_HighlightInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT_AURA_API IEnemy_HighlightInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HighlightActor() = 0;
	virtual void UnhighlightActor() = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const; 
};
