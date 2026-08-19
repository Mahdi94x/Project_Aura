// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "Aura_AbilityTypes.h"
#include "Aura_GameplayTags.h"
#include "AbilitySystem/Aura_AbilitySystemLibrary.h"
#include "AbilitySystem/Aura_AttributeSet.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics /*Raw Internal Struct*/
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)
	
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCapturedDefMap;
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAura_AttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAura_AttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAura_AttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAura_AttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAura_AttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAura_AttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAura_AttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAura_AttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAura_AttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAura_AttributeSet, PhysicalResistance, Target, false);
		
		const FAura_GameplayTags& GameplayTags = FAura_GameplayTags::Get();
		
		TagsToCapturedDefMap.Add(GameplayTags.Attributes_Secondary_Armor, ArmorDef);
		TagsToCapturedDefMap.Add(GameplayTags.Attributes_Secondary_BlockChance, BlockChanceDef);
		TagsToCapturedDefMap.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCapturedDefMap.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCapturedDefMap.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCapturedDefMap.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
		TagsToCapturedDefMap.Add(GameplayTags.Attributes_Secondary_FireResistance, FireResistanceDef);
		TagsToCapturedDefMap.Add(GameplayTags.Attributes_Secondary_ArcaneResistance, ArcaneResistanceDef);
		TagsToCapturedDefMap.Add(GameplayTags.Attributes_Secondary_LightningResistance, LightningResistanceDef);
		TagsToCapturedDefMap.Add(GameplayTags.Attributes_Secondary_PhysicalResistance, PhysicalResistanceDef);
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
	RelevantAttributesToCapture.Add(GetDamageStatic().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().FireResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().LightningResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().PhysicalResistanceDef);
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
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	/*Get Damage SetByCaller Magnitude through the Data Tag for each DamageType and compare it to resistance*/
	float Damage = 0.f;
	
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair  : FAura_GameplayTags::Get().DamageToResistancesTagsMap)
	{
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);
		
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		checkf(AuraDamageStatics().TagsToCapturedDefMap.Contains(ResistanceTag), TEXT("TagsToCapturedDefMap does not contain Tag %s in Exec_Calc_Damage"), *ResistanceTag.ToString());
		
		const FGameplayEffectAttributeCaptureDefinition CapturedResistanceDef = AuraDamageStatics().TagsToCapturedDefMap[ResistanceTag];
		float ResistanceValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CapturedResistanceDef, EvaluationParameters, ResistanceValue);
		ResistanceValue = FMath::Clamp(ResistanceValue, 0.f, 100.f);
		
		DamageTypeValue*= (100.f - ResistanceValue) / 100.f;
		Damage += DamageTypeValue; 
	}
	
	/*Capturing BlockChance on the target, and determine if there was a successful Block
	 * if block, halve the damage*/
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);
	
	const bool bBlocked = FMath::RandRange(1 , 100) < TargetBlockChance;
	UAura_AbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	Damage = bBlocked ? Damage / 2.f : Damage;
	
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
	
	/*Critical Hit Calculation and Coefficients*/
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);
	
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);
	
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);
	
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfoDa->DamageCalculationCoefficient->FindCurve(FName("CriticalHitResistance"),FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetCharacterLevel());
	
	/*CriticalHitResistance reduces CriticalHitChance by a certain percentage */
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1 , 100) < EffectiveCriticalHitChance;
	UAura_AbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	/*Double the damage plus a bonus if critical hit*/
	Damage = bCriticalHit ? (Damage * 2) + SourceCriticalHitDamage : Damage;

	/*Execution*/
	const FGameplayModifierEvaluatedData EvaluatedData(
		UAura_AttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive, 
		Damage);
	
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
