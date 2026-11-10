// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Aura_BaseWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

UCLASS()
class PROJECT_AURA_API USpellMenuWidgetController : public UAura_BaseWidgetController
{
	GENERATED_BODY()
	
public:
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
};
