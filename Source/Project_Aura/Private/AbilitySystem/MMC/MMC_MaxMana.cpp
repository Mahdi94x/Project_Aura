// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "AbilitySystem/MMC/MMC_MaxMana.h"

#include "AbilitySystem/Aura_AttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	// Capture the attribute magnitude value
	IntelDef.AttributeToCapture = UAura_AttributeSet::GetIntelligenceAttribute();
	IntelDef.bSnapshot = false;
	IntelDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	RelevantAttributesToCapture.Add(IntelDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather Tags from source and targets
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Retrieve the captured attribute in the constructor
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	float Intel = 0.f;
	GetCapturedAttributeMagnitude(IntelDef, Spec, EvaluationParameters, Intel);
	Intel = FMath::Max<float>(Intel,0.f);

	// Capture the Level
	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}
	
	// Final Calculation
	return 50.f + 2.5f * Intel + 15.f * PlayerLevel;
}
