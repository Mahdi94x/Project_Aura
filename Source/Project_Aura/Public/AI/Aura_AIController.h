// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Aura_AIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class PROJECT_AURA_API AAura_AIController : public AAIController
{
	GENERATED_BODY()

public:
	AAura_AIController();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

public:
	virtual void Tick(float DeltaTime) override;
};
