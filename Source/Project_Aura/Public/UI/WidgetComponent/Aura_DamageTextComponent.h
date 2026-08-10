// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Aura_DamageTextComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_AURA_API UAura_DamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UAura_DamageTextComponent();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage);
	
};
