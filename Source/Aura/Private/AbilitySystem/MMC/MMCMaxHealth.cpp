// Copyright William Marietta


#include "AbilitySystem/MMC/MMCMaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"


UMMCMaxHealth::UMMCMaxHealth() 
{
	VigorAttributeDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorAttributeDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorAttributeDef);

}

float UMMCMaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;



	float Vigor = 0.0f;
	GetCapturedAttributeMagnitude(VigorAttributeDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max(Vigor, 0.0f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	if (CombatInterface == nullptr) 
	{
		return 0.0f;
	}

	const int32 CombatLevel = CombatInterface->GetCombatLevel();


	const float Value = 90.0f + (Vigor * 5.0f) + (CombatLevel * 10.0f);


	return Value;

}
