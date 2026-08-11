// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "Aura_GameplayTags.h"
#include "AbilitySystem/Aura_AttributeSet.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics /*Raw Internal Struct*/
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAura_AttributeSet, Armor, Target, false);
	}
};

/*A function to return a static variable of the raw struct - 
the only instance (object) of the struct - has static storage duration*/
static const AuraDamageStatics& GetDamageStatic() 
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(GetDamageStatic().ArmorDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	/* Retrieving Data - BoilerPlate Code*/
	const UAbilitySystemComponent* SourceAsc = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAsc = ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourceAvatar = SourceAsc? SourceAsc->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = SourceAsc? TargetAsc->GetAvatarActor() : nullptr;
	
	TScriptInterface<ICombatInterface> SourceCombatInterface = SourceAvatar;
	TScriptInterface<ICombatInterface> TargetCombatInterface = TargetAvatar;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	/*Get Damage SetByCaller Magnitude through the Data Tag*/
	float Damage = Spec.GetSetByCallerMagnitude(FAura_GameplayTags::Get().Damage);
	
	/*Performing Calculations*/
	/*float Armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max<float>(0.f, Armor);
	++Armor;*/
	
	/*Execution the calculations*/
	const FGameplayModifierEvaluatedData EvaluatedData(
		UAura_AttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive, 
		Damage);
	
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
