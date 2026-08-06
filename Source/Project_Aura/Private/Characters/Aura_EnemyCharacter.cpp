// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "Characters/Aura_EnemyCharacter.h"
#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "AbilitySystem/Aura_AbilitySystemLibrary.h"
#include "AbilitySystem/Aura_AttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Project_Aura/Project_Aura.h"
#include "UI/Widget/Aura_UserWidget.h"

AAura_EnemyCharacter::AAura_EnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	Weapon->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAura_AbilitySystemComponent>("EnemyAbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UAura_AttributeSet>("EnemyAttributeSet");
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAura_EnemyCharacter::HighlightActor()
{
	this->GetMesh()->SetRenderCustomDepth(true);
	this->GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAura_EnemyCharacter::UnhighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AAura_EnemyCharacter::GetCharacterLevel()
{
	return this->Level;
}

void AAura_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializeAbilityActorInfo();
	EnemyHealthBarUtilFunc();
}

void AAura_EnemyCharacter::InitializeAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UAura_AbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	InitializeDefaultAttributes();
}

void AAura_EnemyCharacter::InitializeDefaultAttributes() const
{
	UAura_AbilitySystemLibrary::InitializeDefaultAttributes(this, this->CharacterClass, this->Level, this->AbilitySystemComponent);
}

void AAura_EnemyCharacter::EnemyHealthBarUtilFunc()
{
	if (UAura_UserWidget* AuraUserWidget = Cast<UAura_UserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	
	if (const UAura_AttributeSet* AuraAttributeSet = Cast<UAura_AttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});
		
		OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	}
}
