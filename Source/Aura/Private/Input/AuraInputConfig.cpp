// Copyright William Marietta


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::GetAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	if(AbilityInputActionsMap.Contains(InputTag))
	{
		return AbilityInputActionsMap[InputTag].InputAction;
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input tag '%s' not found in AbilityInputActionsMap."), *InputTag.ToString());
	}


	return nullptr;
}
