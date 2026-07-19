// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "PlayerController/Aura_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/HighlightInterface.h"

AAura_PlayerController::AAura_PlayerController()
{
	bReplicates = true;
}

void AAura_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAura_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	checkf(AuraMappingContext,TEXT("AuraMappingContext is not set in the details panel"));
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()); IsValid(Subsystem))
	{
		Subsystem->AddMappingContext(AuraMappingContext,0);
	}
	
	/*Top-Down Properties*/
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAura_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(AuraMoveAction, ETriggerEvent::Triggered, this, &ThisClass::AuraMove);
	
}

void AAura_PlayerController::AuraMove(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}

void AAura_PlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	if (!CursorHit.bBlockingHit) return;
	
	LastFrameActor = ThisFrameActor;
	ThisFrameActor = CursorHit.GetActor();

	/* *Line Trace From Cursor, there are several scenarios
	 * A. LastFrameActor is null, and ThisFrameActor is null
	 *		-> Do Nothing, empty else case
	 * B. LastFrameActor is null, and ThisFrameActor is valid
	 *		-> Highlight ThisFrameActor
	 * C. LastFrameActor is valid, and ThisFrameActor is null
	 *		-> UnHighlight LastFrameActor
	 * D. Both Actors are valid, but LastFrameActor != ThisFrameActor (Switching)
	 *		-> Unhighlight LastFrameActor, Highlight ThisFrameActor
	 * E. Both Actors are valid and are the same actor LastFrameActor == ThisFrameActor
	 *		-> Do nothing
	 */

	if (LastFrameActor == nullptr)
	{
		if (ThisFrameActor != nullptr)
		{
			// Case B
			ThisFrameActor->HighlightActor();
		}
		else 
		{
			// Both Actors are null, Case A -> Do Nothing
		}
	}
	else // LastFrameActor is valid
	{
		if (ThisFrameActor == nullptr)
		{
			// Case C
			LastFrameActor->UnhighlightActor();
		}
		else // Both Actors are valid
		{
			if (LastFrameActor != ThisFrameActor)
			{
				// Case D
				LastFrameActor->UnhighlightActor();
				ThisFrameActor->HighlightActor();
			}
			else 
			{
				//  both actors are valid, Same Actors, Case E -> Do Nothing
			}
		}
	}
}
