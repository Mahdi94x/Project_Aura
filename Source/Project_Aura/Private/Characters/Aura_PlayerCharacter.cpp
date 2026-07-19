// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "Characters/Aura_PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerController/Aura_PlayerController.h"
#include "PlayerState/Aura_PlayerState.h"
#include "UI/HUD/Aura_HUD.h"

AAura_PlayerCharacter::AAura_PlayerCharacter()
{
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
	
	/*to bind the delegates of the gameplay effect - the character class depend on the ASC class but not vice versa*/
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

int32 AAura_PlayerCharacter::GetCharacterLevel()
{
	const AAura_PlayerState* AuraPlayerState = GetPlayerState<AAura_PlayerState>(); 
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerStateLevel();
}
