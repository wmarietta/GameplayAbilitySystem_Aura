// Copyright William Marietta


#include "AbilitySystem/AuraAbilitySystemComponent.h"



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

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<class UGameplayAbility>>& AbilitiesToAdd)
{
	for(const TSubclassOf<class UGameplayAbility>& Ability : AbilitiesToAdd)
	{
		if (Ability)
		{
			FGameplayAbilitySpec AbilitySpec(Ability, 1);
			//GiveAbility(AbilitySpec);
			GiveAbilityAndActivateOnce(AbilitySpec);
		}
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


