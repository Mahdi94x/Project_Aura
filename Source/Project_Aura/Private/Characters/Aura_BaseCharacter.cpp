// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "Characters/Aura_BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Aura_AbilitySystemComponent.h"

AAura_BaseCharacter::AAura_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

int32 AAura_BaseCharacter::GetCharacterLevel()
{
	return 0;
}

void AAura_BaseCharacter::InitializeAbilityActorInfo()
{
	
}

void AAura_BaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const 
{
	check(IsValid(GetAbilitySystemComponent()));
	checkf(GameplayEffectClass, TEXT("Add Default Attribute GameplayEffect in the details panel"));
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void AAura_BaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AAura_BaseCharacter::AddCharacterAbilities()
{
	UAura_AbilitySystemComponent* AuraAsc = CastChecked<UAura_AbilitySystemComponent>(this->AbilitySystemComponent);
	if (!IsValid(AuraAsc)) return;
	if (!HasAuthority()) return;
	
	AuraAsc->GrantCharacterAbilities(this->StartupAbilitiesArray);
}

UAbilitySystemComponent* AAura_BaseCharacter::GetAbilitySystemComponent() const
{
	return this->AbilitySystemComponent;
}

UAttributeSet* AAura_BaseCharacter::GetAttributeSet() const
{
	return this->AttributeSet;
}

void AAura_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}


