// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AbilitySystem/ASyncTasks/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent,
                                                                const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitCooldownChangeObject = NewObject<UWaitCooldownChange>();
	WaitCooldownChangeObject->Asc = InAbilitySystemComponent;
	WaitCooldownChangeObject->CooldownTag = InCooldownTag;
	
	if (!IsValid(InAbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChangeObject->EndTask();
		return nullptr;
	}
	
	// To know when a cooldown effect has been expired (end), cooldown tag has been removed
	InAbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
		WaitCooldownChangeObject, &UWaitCooldownChange::CooldownTagChanged);
	
	// To know when a cooldown effect has been applied (start)
	InAbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject
		(WaitCooldownChangeObject, &UWaitCooldownChange::OnActiveEffectAdded);
	
	return WaitCooldownChangeObject;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(Asc)) return;
	
	Asc->RegisterGameplayTagEvent(CooldownTag,EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetAsc,
	const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);
	
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);
	
	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesArray = Asc->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		
		if (TimesArray.Num() > 0)
		{
			float TimeRemaining = TimesArray[0];
			for (int32 i =0; i < TimesArray.Num(); i++)
			{
				if (TimesArray[i] > TimeRemaining)
				{
					TimeRemaining = TimesArray[i];
				}
			}
			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}
