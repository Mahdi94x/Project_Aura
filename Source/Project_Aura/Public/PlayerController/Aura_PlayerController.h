// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "Aura_PlayerController.generated.h"

class USplineComponent;
class UAura_InputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IHighlightInterface;
class UAura_AbilitySystemComponent;

UCLASS()
class PROJECT_AURA_API AAura_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAura_PlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void AuraMove (const FInputActionValue& InputActionValue);
	void CursorTrace();
	UAura_AbilitySystemComponent* GetAuraAsc();
	
	/*Callback Functions for InputActions associated with FGameplayTags*/
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditAnywhere, Category = "Aura|Input")
	TObjectPtr<UInputMappingContext> AuraMappingContext;

	UPROPERTY(EditAnywhere, Category = "Aura|Input")
	TObjectPtr<UInputAction> AuraMoveAction;

	TScriptInterface<IHighlightInterface> LastFrameActor;
	TScriptInterface<IHighlightInterface> ThisFrameActor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Aura|Input")
	TObjectPtr<UAura_InputConfig> InputConfigDataAsset;
	
	UPROPERTY()
	TObjectPtr<UAura_AbilitySystemComponent> AuraAbilitySystemComponent;
	
	/*ClickToMove - AutoRunning*/
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Aura|Movement")
	float AutoRunAcceptanceRadius = 50.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
};