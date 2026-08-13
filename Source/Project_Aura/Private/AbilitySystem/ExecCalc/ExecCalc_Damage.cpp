// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "Aura_GameplayTags.h"
#include "AbilitySystem/Aura_AbilitySystemLibrary.h"
#include "AbilitySystem/Aura_AttributeSet.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics /*Raw Internal Struct*/
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAura_AttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAura_AttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAura_AttributeSet, ArmorPenetration, Source, false);
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
	RelevantAttributesToCapture.Add(GetDamageStatic().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().ArmorPenetrationDef);
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
	
	/*Capturing BlockChance on the target, and determine if there was a successful Block
	 * if block, halve the damage*/
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);
	if (const bool bBlocked = FMath::RandRange(1 , 100) < TargetBlockChance) Damage *= 0.5f;
	
	/*Capturing TargetArmor and SourceArmorPenetration - Caching Coefficients*/
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);
	
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);
	
	const UCharacterClassInfo* CharacterClassInfoDa = UAura_AbilitySystemLibrary::GetCharacterClassInfoDa(SourceAvatar);
	
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfoDa->DamageCalculationCoefficient->FindCurve(FName("ArmorPenetration"),FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetCharacterLevel());
	
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfoDa->DamageCalculationCoefficient->FindCurve(FName("EffectiveArmor"),FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetCharacterLevel());
	
	/*SourceArmorPenetration ignores a percentage of the TargetArmor, evaluating EffectiveArmor*/
	const float EffectiveArmor = TargetArmor *= (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	/*Execution*/
	const FGameplayModifierEvaluatedData EvaluatedData(
		UAura_AttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive, 
		Damage);
	
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
