// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Aura_BaseCharacter.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Aura_EnemyCharacter.generated.h"

class UBehaviorTree;
class AAura_AIController;
class UWidgetComponent;

UCLASS()
class PROJECT_AURA_API AAura_EnemyCharacter : public AAura_BaseCharacter, public IHighlightInterface
{
	GENERATED_BODY()
	
public:
	AAura_EnemyCharacter();
	virtual void BeginPlay() override;
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void PossessedBy(AController* NewController) override;
	
	UPROPERTY(BlueprintAssignable, Category="Aura|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Aura|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintReadOnly, Category = "Aura|Combat")
	bool bHitReacting = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Aura|Combat")
	float BaseWalkSpeed = 250.f;
	
	// ~ Begin IHighlight Interface
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;
	// ~ End IHighlight Interface

	// ~ Begin ICombat Interface
	virtual int32 GetCharacterLevel() override;
	virtual void Die() override;
	// ~ End ICombat Interface

protected:
	virtual void InitializeAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|CharacterClassDefaults")
	int32 Level = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|CharacterClassDefaults")
	ECharacterClass CharacterClass = ECharacterClass::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Combat")
	float LifeSpan = 5.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(EditAnywhere, Category="Aura|Combat")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY()
	TObjectPtr<AAura_AIController> AuraAIController;
	
private:
	void EnemyHealthBarUtilFunc();
	
	
	
};
