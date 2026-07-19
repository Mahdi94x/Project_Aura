// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "Characters/Aura_EnemyCharacter.h"
#include "AbilitySystem/Aura_AbilitySystemComponent.h"
#include "AbilitySystem/Aura_AttributeSet.h"
#include "Project_Aura/Project_Aura.h"

AAura_EnemyCharacter::AAura_EnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	Weapon->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAura_AbilitySystemComponent>("EnemyAbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UAura_AttributeSet>("EnemyAttributeSet");
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
}

void AAura_EnemyCharacter::InitializeAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	/*to bind the delegates of the gameplay effect - the character class depend on the ASC class but not vice versa*/
	Cast<UAura_AbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}
