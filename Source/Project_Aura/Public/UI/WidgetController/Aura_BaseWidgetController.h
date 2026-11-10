// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Aura_BaseWidgetController.generated.h"

class UAbilityInfo;
class UAura_AttributeSet;
class UAura_AbilitySystemComponent;
class AAura_PlayerState;
class AAura_PlayerController;
class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	
	FWidgetControllerParams() {} // default constructor
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC),PlayerState(PS) , AbilitySystemComponent(ASC), AttributeSet(AS) {} // parametrized constructor
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInfoChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, Info);

UCLASS()
class PROJECT_AURA_API UAura_BaseWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	
	virtual void BindCallbacksToDependencies();
	
	UPROPERTY(BlueprintAssignable, Category="Aura|Messages")
	FAbilityInfoSignature AbilityInfoDelegate;
	
	void BroadcastAbilityInfo();

protected:
	
	AAura_PlayerController* GetAuraPC();
	AAura_PlayerState* GetAuraPS();
	UAura_AbilitySystemComponent* GetAuraAsc();
	UAura_AttributeSet* GetAuraAs();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	UPROPERTY(BlueprintReadOnly, Category = "Aura|WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category = "Aura|WidgetController")
	TObjectPtr<APlayerState> PlayerState;
	
	UPROPERTY(BlueprintReadOnly, Category = "Aura|WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Aura|WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly, Category = "Aura|WidgetController")
	TObjectPtr<AAura_PlayerController> AuraPlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category = "Aura|WidgetController")
	TObjectPtr<AAura_PlayerState> AuraPlayerState;
	
	UPROPERTY(BlueprintReadOnly, Category = "Aura|WidgetController")
	TObjectPtr<UAura_AbilitySystemComponent> AuraAbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Aura|WidgetController")
	TObjectPtr<UAura_AttributeSet> AuraAttributeSet;
	
};
