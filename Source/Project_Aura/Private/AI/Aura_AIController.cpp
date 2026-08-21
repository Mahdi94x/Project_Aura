// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AI/Aura_AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AAura_AIController::AAura_AIController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	check(Blackboard);
	
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	check(BehaviorTreeComponent);
}

void AAura_AIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAura_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

