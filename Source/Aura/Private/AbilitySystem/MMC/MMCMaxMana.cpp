// Copyright William Marietta


#include "AbilitySystem/MMC/MMCMaxMana.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"



UMMCMaxMana::UMMCMaxMana()
{
	IntelligenceAttributeDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceAttributeDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceAttributeDef);
}

float UMMCMaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{

	//gather tags from the source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intelligence = 0.0f;
	GetCapturedAttributeMagnitude(IntelligenceAttributeDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max(Intelligence, 0.0f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	if(CombatInterface == nullptr)
	{
		return 0.0f;
	}

	const int32 CombatLevel = CombatInterface->GetCombatLevel();


	const float Value = 90.0f + (Intelligence * 5.0f) + (CombatLevel * 10.0f);


	return Value;
}
