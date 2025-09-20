// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "PlayerController/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/HighlightInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraMappingContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraMappingContext,0);
	}
	
	
	/*Top-Down Properties*/
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(true);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(AuraMoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::AuraMove);
	}

}

void AAuraPlayerController::AuraMove(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	if (!CursorHit.bBlockingHit) return;
	
	LastFrameActor = ThisFrameActor;
	ThisFrameActor = CursorHit.GetActor();

	/* *Line Trace From Cursor, there are several scenarios
	 * A. LastFrameActor is null and ThisFrameActor is null
	 *		-> Do Nothing, empty else case
	 * B. LastFrameActor is null and ThisFrameActor is valid
	 *		-> Highlight ThisFrameActor
	 * C. LastFrameActor is valid and ThisFrameActor is null
	 *		-> UnHighlight LastFrameActor
	 * D. Both Actors are valid but LastFrameActor!= ThisFrameActor (Switching)
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
			// Both Actors are null 
			// Case A -> Do Nothing
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
				//  both actors are valid, Same Actors
				// Case E -> Do Nothing
			}
		}
	}
	
}
