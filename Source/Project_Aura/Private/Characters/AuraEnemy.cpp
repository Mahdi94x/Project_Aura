// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "Characters/AuraEnemy.h"

#include "Project_Aura/Project_Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	Weapon->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
}

void AAuraEnemy::HighlightActor()
{
	UE_LOG(LogTemp, Warning, TEXT("Highlight The Actor"));

	this->GetMesh()->SetRenderCustomDepth(true);
	this->GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnhighlightActor()
{
	UE_LOG(LogTemp, Warning, TEXT("Unhighlight The Actor"));

	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);

}
