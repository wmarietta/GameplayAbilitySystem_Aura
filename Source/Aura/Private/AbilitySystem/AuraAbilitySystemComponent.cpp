// Copyright William Marietta


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::RegisterToAbilitySystemDelegates()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, const FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	for (auto Tag : TagContainer) 
	{
		//TODO: Broadcast tag to widget controller
		UE_LOG(LogTemp, Warning, TEXT("Effect Applied: %s"), *Tag.ToString());
	}
}
