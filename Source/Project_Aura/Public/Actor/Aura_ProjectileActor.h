// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "Aura_ProjectileActor.generated.h"

struct FGameplayEffectSpecHandle;
class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;
class UAudioComponent;

UCLASS()
class PROJECT_AURA_API AAura_ProjectileActor : public AActor
{
	GENERATED_BODY()

public:
	AAura_ProjectileActor();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	virtual void Destroyed() override;
	
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category="Aura|ProjectileActor")
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereOverlap;
	
	UPROPERTY(EditAnywhere, Category="Aura|ProjectileActor")
	TObjectPtr<USoundBase> LoopSound;
	
	UPROPERTY(EditAnywhere, Category="Aura|ProjectileActor")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditAnywhere, Category="Aura|ProjectileActor")
	TObjectPtr<USoundBase> ImpactSound;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAudioComponent> LoopingSoundComponent; 
	
	UPROPERTY(EditDefaultsOnly, Category="Aura|ProjectileActor")
	float LifeSpan = 15.f;
	
	bool bHit = false;
};
