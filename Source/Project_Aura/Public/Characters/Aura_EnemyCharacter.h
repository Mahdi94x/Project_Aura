// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Aura_BaseCharacter.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Aura_EnemyCharacter.generated.h"

class UWidgetComponent;

UCLASS()
class PROJECT_AURA_API AAura_EnemyCharacter : public AAura_BaseCharacter, public IHighlightInterface
{
	GENERATED_BODY()
public:
	AAura_EnemyCharacter();
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintAssignable, Category="Aura|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Aura|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	// ~ Begin IHighlight Interface
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;
	// ~ End IHighlight Interface

	// ~ Begin ICombat Interface
	virtual int32 GetCharacterLevel() override;
	// ~ End ICombat Interface

protected:
	virtual void InitializeAbilityActorInfo() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|CharacterClassDefaults")
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
private:
	void EnemyHealthBarUtilFunc();
	
};
