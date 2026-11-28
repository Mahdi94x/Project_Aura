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
	/*Declare the attribute to capture*/
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
	
	AuraDamageStatics() /*The constructor of the struct - define the attribute to capture*/
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
	}
};

/*A function to return a static variable of the raw struct - 
the only instance (object) of the struct - has static storage duration*/
static const AuraDamageStatics& GetDamageStatic() 
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, 
	FAggregatorEvaluateParameters EvaluationParameters, 
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	const FAura_GameplayTags GameplayTags = FAura_GameplayTags::Get();
	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageToDebuffTagsMap)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		/*Getting the damage value from the spec*/
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType,false, -1.f);
		
		if (TypeDamage > -0.5f) /*0.5 padding for floating point [im]precision*/
		{
			// determine if there is a successful debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1.f);
			
			/*resistance*/
			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageToResistancesTagsMap[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude
				(InTagsToDefs[ResistanceTag], EvaluationParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			
			/*EffectiveDebuffChance*/
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.f;
			const bool bDebuff = FMath::RandRange(1,100) < EffectiveDebuffChance;
			if (bDebuff)
			{
				// TODO: What do we do ?
			}
		}
	}
}

UExecCalc_Damage::UExecCalc_Damage() 
/*The Constructor of the exec_calc, considering the attributes to capture into calculations*/
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
	/*
	 *local TMap instance to bind the captured attribute to the relevant tags
	 */
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCapturedDefs;
	const FAura_GameplayTags& GameplayTags = FAura_GameplayTags::Get();
	TagsToCapturedDefs.Add(GameplayTags.Attributes_Secondary_Armor, GetDamageStatic().ArmorDef);
	TagsToCapturedDefs.Add(GameplayTags.Attributes_Secondary_BlockChance, GetDamageStatic().BlockChanceDef);
	TagsToCapturedDefs.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetDamageStatic().ArmorPenetrationDef);
	TagsToCapturedDefs.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetDamageStatic().CriticalHitChanceDef);
	TagsToCapturedDefs.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetDamageStatic().CriticalHitDamageDef);
	TagsToCapturedDefs.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetDamageStatic().CriticalHitResistanceDef);
		
	TagsToCapturedDefs.Add(GameplayTags.Attributes_Secondary_FireResistance, GetDamageStatic().FireResistanceDef);
	TagsToCapturedDefs.Add(GameplayTags.Attributes_Secondary_ArcaneResistance, GetDamageStatic().ArcaneResistanceDef);
	TagsToCapturedDefs.Add(GameplayTags.Attributes_Secondary_LightningResistance, GetDamageStatic().LightningResistanceDef);
	TagsToCapturedDefs.Add(GameplayTags.Attributes_Secondary_PhysicalResistance, GetDamageStatic().PhysicalResistanceDef);
	
	/* 
	 * Retrieving Data - BoilerPlate Code
	 */
	const UAbilitySystemComponent* SourceAsc = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAsc = ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourceAvatar = SourceAsc? SourceAsc->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = SourceAsc? TargetAsc->GetAvatarActor() : nullptr;
	
	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetCharacterLevel(SourceAvatar);
	}
	
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetCharacterLevel(TargetAvatar);
	}
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	/*
	 * Debuff
	 */
	DetermineDebuff(ExecutionParams, Spec, EvaluationParameters, TagsToCapturedDefs);
	
	/*
	 * Get Damage SetByCaller Magnitude through the Data Tag for each DamageType and compare it to resistance
	 */
	float Damage = 0.f;
	
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FAura_GameplayTags::Get().DamageToResistancesTagsMap)
	{
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);
		
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		checkf(TagsToCapturedDefs.Contains(ResistanceTag), 
			TEXT("TagsToCapturedDefMap does not contain Tag %s in Exec_Calc_Damage"), *ResistanceTag.ToString());
		
		const FGameplayEffectAttributeCaptureDefinition CapturedResistanceDef = TagsToCapturedDefs[ResistanceTag];
		float ResistanceValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CapturedResistanceDef, EvaluationParameters, ResistanceValue);
		ResistanceValue = FMath::Clamp(ResistanceValue, 0.f, 100.f);
		
		DamageTypeValue*= (100.f - ResistanceValue) / 100.f;
		Damage += DamageTypeValue; 
	}
	
	/*
	 *Capturing BlockChance on the target, and determine if there was a successful Block
	 * if block, halve the damage
	 */
	float TargetBlockChance = 0.f;
	const FGameplayTag& BlockChanceTag = GameplayTags.Attributes_Secondary_BlockChance;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCapturedDefs[BlockChanceTag], EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);
	
	const bool bBlocked = FMath::RandRange(1 , 100) < TargetBlockChance;
	UAura_AbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	Damage = bBlocked ? Damage / 2.f : Damage;
	
	/*
	 * Capturing TargetArmor and SourceArmorPenetration - Caching Coefficients
	 */
	float TargetArmor = 0.f;
	const FGameplayTag& ArmorTag = GameplayTags.Attributes_Secondary_Armor;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCapturedDefs[ArmorTag], EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);
	
	float SourceArmorPenetration = 0.f;
	const FGameplayTag& ArmorPenetrationTag = GameplayTags.Attributes_Secondary_ArmorPenetration;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCapturedDefs[ArmorPenetrationTag], EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);
	
	const UCharacterClassInfo* CharacterClassInfoDa = UAura_AbilitySystemLibrary::GetCharacterClassInfoDa(SourceAvatar);
	
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfoDa->DamageCalculationCoefficient->FindCurve(FName("ArmorPenetration"),FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
	
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfoDa->DamageCalculationCoefficient->FindCurve(FName("EffectiveArmor"),FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);
	
	/*
	 *SourceArmorPenetration ignores a percentage of the TargetArmor, evaluating EffectiveArmor
	 */
	const float EffectiveArmor = TargetArmor *= (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;
	
	/*
	 *Critical Hit Calculation and Coefficients
	 */
	float SourceCriticalHitChance = 0.f;
	const FGameplayTag& CriticalHitChanceTag = GameplayTags.Attributes_Secondary_CriticalHitChance;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude
		(TagsToCapturedDefs[CriticalHitChanceTag], EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);
	
	float TargetCriticalHitResistance = 0.f;
	const FGameplayTag& CriticalHitResistanceTag = GameplayTags.Attributes_Secondary_CriticalHitResistance;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude
		(TagsToCapturedDefs[CriticalHitResistanceTag], EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);
	
	float SourceCriticalHitDamage = 0.f;
	const FGameplayTag& CriticalHitDamageTag = GameplayTags.Attributes_Secondary_CriticalHitDamage;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude
		(TagsToCapturedDefs[CriticalHitDamageTag], EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);
	
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfoDa->DamageCalculationCoefficient->FindCurve(FName("CriticalHitResistance"),FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);
	
	/*
	 *CriticalHitResistance reduces CriticalHitChance by a certain percentage
	 */
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1 , 100) < EffectiveCriticalHitChance;
	UAura_AbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	/*
	 *Double the damage plus a bonus if critical hit
	 */
	Damage = bCriticalHit ? (Damage * 2) + SourceCriticalHitDamage : Damage;

	/*
	 * Execution
	 */
	const FGameplayModifierEvaluatedData EvaluatedData(
		UAura_AttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive, 
		Damage);
	
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
