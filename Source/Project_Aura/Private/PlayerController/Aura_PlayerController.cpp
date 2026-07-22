// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "PlayerController/Aura_PlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Aura_GameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/Aura_InputComponent.h"
#include "Interaction/HighlightInterface.h"

AAura_PlayerController::AAura_PlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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
	UAura_InputComponent* AuraInputComponent = CastChecked<UAura_InputComponent>(InputComponent);
	
	AuraInputComponent->BindAction(AuraMoveAction, ETriggerEvent::Triggered, this, &ThisClass::AuraMove);
	AuraInputComponent->BindAbilityActions(InputConfigDataAsset, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	
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

UAura_AbilitySystemComponent* AAura_PlayerController::GetAuraAsc()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAura_AbilitySystemComponent>
		(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

/**
 * LMB Input Action/Tag will be vital, a lot of variables will determine its purpose.
 * an ability could be assigned to it @ the start and then remapping another ability @ runtime.
 * LMB will also control the movement of the character, 
 * held => AutoRun.
 * pressed/released on the floor => ClickToMove.
 * pressed/released on the CombatInterFace => Attack / Activate an ability.
 * short release will be determined by comparing to ShortPressThreshold.
 */
void AAura_PlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAura_GameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisFrameActor ? true : false; /*Attack vs. Movement*/
		bAutoRunning = false; /*if the press is short, the AutoRunning behavior will be implemented in ReleasedFunction*/
	}
}

void AAura_PlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetAuraAsc() == nullptr) return;
	GetAuraAsc()->AbilityInputTagReleased(InputTag);
}

void AAura_PlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	/*Not LMB, TryActivateAbility*/
	if (!InputTag.MatchesTagExact(FAura_GameplayTags::Get().InputTag_LMB)) 
	{
		if (GetAuraAsc())
		{
			GetAuraAsc()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	
	// LMB => ClickToMove if targeting is false, otherwise TryActivateAbility (Attack)
	if (bTargeting)
	{
		if (GetAuraAsc())
		{
			GetAuraAsc()->AbilityInputTagHeld(InputTag);
		}
	}
	else /*Hold Click To Move*/
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		FHitResult Hit;
		
		if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
		{
			CachedDestination = Hit.ImpactPoint;
		}
		
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

