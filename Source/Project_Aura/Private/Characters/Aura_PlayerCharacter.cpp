// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "Characters/Aura_PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerController/Aura_PlayerController.h"
#include "PlayerState/Aura_PlayerState.h"
#include "UI/HUD/Aura_HUD.h"

AAura_PlayerCharacter::AAura_PlayerCharacter()
{
	CharacterClass = ECharacterClass::Elementalist;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
}

void AAura_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//server
	InitializeAbilityActorInfo();
	AddCharacterAbilities();
}

void AAura_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//client
	InitializeAbilityActorInfo();
}

void AAura_PlayerCharacter::InitializeAbilityActorInfo()
{
	// function from the APawn class to get the player state in the AuraCharacter class
	AAura_PlayerState* AuraPlayerState = GetPlayerState<AAura_PlayerState>(); 
	check(AuraPlayerState);
	
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	
	/*to bind the delegates of the gameplay effect, the character class depend on the ASC class but not vice versa*/
	Cast<UAura_AbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
	if (AAura_PlayerController* AuraPlayerController = Cast<AAura_PlayerController>(GetController()))
	{
		if (AAura_HUD* AuraHUD = Cast<AAura_HUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, this->AbilitySystemComponent, this->AttributeSet);
		}
	}
	
	this->InitializeDefaultAttributes();
}

void AAura_PlayerCharacter::LevelUp_Implementation()
{
	
}

int32 AAura_PlayerCharacter::GetCharacterLevel_Implementation()
{
	const AAura_PlayerState* AuraPlayerState = GetPlayerState<AAura_PlayerState>(); 
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerStateLevel();
}

int32 AAura_PlayerCharacter::GetXP_Implementation() const
{
	const AAura_PlayerState* AuraPlayerState = GetPlayerState<AAura_PlayerState>(); 
	check(AuraPlayerState);
	
	return AuraPlayerState->GetXP();
}

int32 AAura_PlayerCharacter::FindLevelFromXP_Implementation(int32 InXP) const
{
	const AAura_PlayerState* AuraPlayerState = GetPlayerState<AAura_PlayerState>(); 
	check(AuraPlayerState);
	
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AAura_PlayerCharacter::GetAttributePointsReward_Implementation(int32 InLevel) const
{
	const AAura_PlayerState* AuraPlayerState = GetPlayerState<AAura_PlayerState>(); 
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[InLevel].AttributePointAward;
}

int32 AAura_PlayerCharacter::GetSpellPointsReward_Implementation(int32 InLevel) const
{
	const AAura_PlayerState* AuraPlayerState = GetPlayerState<AAura_PlayerState>(); 
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[InLevel].SpellPointAward;
}

void AAura_PlayerCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AAura_PlayerState* AuraPlayerState = GetPlayerState<AAura_PlayerState>(); 
	check(AuraPlayerState);
	
	AuraPlayerState->AddToLevel(InPlayerLevel);
}

void AAura_PlayerCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	// TODO: Add AttributePoints member variable to player state
}

void AAura_PlayerCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	// TODO: Add AttributePoints member variable to player state
}

void AAura_PlayerCharacter::AddToXP_Implementation(int32 InXP)
{
	AAura_PlayerState* AuraPlayerState = GetPlayerState<AAura_PlayerState>(); 
	check(AuraPlayerState);
	
	AuraPlayerState->AddToXP(InXP);
}
