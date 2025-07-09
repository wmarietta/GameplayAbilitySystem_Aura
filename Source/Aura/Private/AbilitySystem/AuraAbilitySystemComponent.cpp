// Copyright William Marietta


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"
#include <Player/AuraPlayerController.h>


void UAuraAbilitySystemComponent::RegisterToAbilitySystemDelegates()
{
	if (GetOwner()->HasAuthority())
	{
		OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::Client_EffectApplied);
	}
	else 
	{
		OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
	}
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, const FActiveGameplayEffectHandle ActiveEffectHandle)
{
	BroadcastEffectAssetTags(EffectSpec);
}

void UAuraAbilitySystemComponent::Client_EffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, const FActiveGameplayEffectHandle ActiveEffectHandle)
{
	BroadcastEffectAssetTags(EffectSpec);
}


void UAuraAbilitySystemComponent::BroadcastEffectAssetTags(const FGameplayEffectSpec& EffectSpec)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	OnEffectAssetTagsAppliedDelegate.Broadcast(TagContainer);
}


