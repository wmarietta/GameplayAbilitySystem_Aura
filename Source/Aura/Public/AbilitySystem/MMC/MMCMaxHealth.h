// Copyright William Marietta

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"

#include "MMCMaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMCMaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	

public:
	UMMCMaxHealth();


	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	FGameplayEffectAttributeCaptureDefinition VigorAttributeDef;

};
