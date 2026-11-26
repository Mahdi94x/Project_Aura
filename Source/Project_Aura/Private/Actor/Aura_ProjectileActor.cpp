// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "Actor/Aura_ProjectileActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/Aura_AbilitySystemLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Project_Aura/Project_Aura.h"


AAura_ProjectileActor::AAura_ProjectileActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	SphereOverlap = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(SphereOverlap);
	SphereOverlap->SetCollisionObjectType(ECC_Projectile);
	SphereOverlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereOverlap->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereOverlap->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereOverlap->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereOverlap->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	
	LoopingSoundComponent = CreateDefaultSubobject<UAudioComponent>("LoopingSoundComponent");
	LoopingSoundComponent->SetupAttachment(GetRootComponent());
	
}

void AAura_ProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(LifeSpan);
	SphereOverlap->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopSound, GetRootComponent());
}

void AAura_ProjectileActor::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if (LoopingSoundComponent) LoopingSoundComponent->Stop();
	bHit = true;
}

void AAura_ProjectileActor::Destroyed()
{
	if (!bHit && !HasAuthority()) OnHit();
	Super::Destroyed();
}

void AAura_ProjectileActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* SourceAvatarActor = DamageEffectParams.SourceAsc->GetAvatarActor();
	
	if (SourceAvatarActor == OtherActor) return;
	if (!UAura_AbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return;
	if (!bHit) OnHit();
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			DamageEffectParams.TargetAsc = TargetAsc;
			UAura_AbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		Destroy();
	}
	else bHit = true;
	
}



