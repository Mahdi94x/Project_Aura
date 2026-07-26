// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "Actor/Aura_ProjectileActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


AAura_ProjectileActor::AAura_ProjectileActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	SphereOverlap = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(SphereOverlap);
	SphereOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereOverlap->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereOverlap->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereOverlap->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereOverlap->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	
}

void AAura_ProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	SphereOverlap->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
}

void AAura_ProjectileActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}



