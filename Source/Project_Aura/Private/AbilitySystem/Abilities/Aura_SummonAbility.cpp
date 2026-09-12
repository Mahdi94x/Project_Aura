// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AbilitySystem/Abilities/Aura_SummonAbility.h"
//#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAura_SummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector(); /*Unit-Vector*/
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpreadAngle/2.f, FVector::UpVector); /*Unit-Vector*/
	const float DeltaSpread = SpawnSpreadAngle/NumOfMinions;
	
	/*const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpreadAngle/2.f, FVector::UpVector); /*Unit-Vector
	
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
		Location, Location + (LeftOfSpread * MaxSpawnDistance), 4.f, FLinearColor::Black, 3.f);
	
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
		Location, Location + (RightOfSpread * MaxSpawnDistance), 4.f, FLinearColor::Black, 3.f);*/
	
	TArray<FVector>	SpawnLocations;
	
	for (int32 i = 0; i < NumOfMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector); /*Unit-Vector*/
		FVector ChosenSpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance); /*Scaled-Vector*/
		
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit,
			ChosenSpawnLocation + FVector(0.f,0.f,400.f),
			ChosenSpawnLocation - FVector(0.f,0.f,400.f),
			ECC_Visibility);
		
		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		
		SpawnLocations.AddUnique(ChosenSpawnLocation);
		
		/*UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
		Location, Location + (Direction * MaxSpawnDistance), 4.f, FLinearColor::Green, 3.f);
		DrawDebugSphere(GetWorld(), Location + (Direction * MinSpawnDistance), 8.f,12,FColor::Red,false,3.f);
		DrawDebugSphere(GetWorld(), Location + (Direction * MaxSpawnDistance), 8.f,12,FColor::Red,false,3.f);
		DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 15.f,12,FColor::Cyan,false,3.f);*/
	}
	
	return SpawnLocations;
}

