// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "PlayerState/Aura_PlayerState.h"
#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "AbilitySystem/Aura_AttributeSet.h"
#include "Net/UnrealNetwork.h"

AAura_PlayerState::AAura_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAura_AbilitySystemComponent>("AuraAbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UAura_AttributeSet>("AuraAttributeSet");
	
	SetNetUpdateFrequency(100.f);
}

void AAura_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAura_PlayerState, Level);
	DOREPLIFETIME(AAura_PlayerState, XP);
}

UAbilitySystemComponent* AAura_PlayerState::GetAbilitySystemComponent() const
{
	return this->AbilitySystemComponent;
}

UAttributeSet* AAura_PlayerState::GetAttributeSet() const
{
	return this->AttributeSet;
}

void AAura_PlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(this->Level);
}

void AAura_PlayerState::SetLevel(int32 NewLevel)
{
	this->Level = NewLevel;
	OnLevelChangedDelegate.Broadcast(this->Level);
}

void AAura_PlayerState::AddToLevel(int32 NewLevel)
{
	this->Level += NewLevel;
	OnLevelChangedDelegate.Broadcast(this->Level);
}

void AAura_PlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(this->XP);
}

void AAura_PlayerState::SetXP(int32 NewXP)
{
	this->XP = NewXP;
	OnXPChangedDelegate.Broadcast(this->XP);
}

void AAura_PlayerState::AddToXP(int32 NewXP)
{
	this->XP += NewXP;
	OnXPChangedDelegate.Broadcast(this->XP);
}

