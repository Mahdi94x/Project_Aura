// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass) const
{
	UAbilitySystemComponent* TargetActorAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetActorAsc) return;

	checkf(GameplayEffectClass, TEXT("Set the GameplayEffectClass in the blueprint details panel"));
	FGameplayEffectContextHandle Context = TargetActorAsc->MakeEffectContext();
	Context.AddSourceObject(this);
	const FGameplayEffectSpecHandle Spec = TargetActorAsc->MakeOutgoingSpec(GameplayEffectClass, 1.f,Context);
	TargetActorAsc->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	
}


