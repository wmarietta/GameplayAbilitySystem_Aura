// Copyright William Marietta


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"



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

/** Adds abilitys to characters*/
void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<class UGameplayAbility>>& AbilitiesToAdd)
{
	for(const TSubclassOf<class UGameplayAbility>& Ability : AbilitiesToAdd)
	{

		if (Ability)
		{
			FGameplayAbilitySpec AbilitySpec(Ability, 1);

			if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability)) 
			{
				AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraAbility->StartupInputTag);
				GiveAbility(AbilitySpec);
			}

		}

	}

}

/** Part of input system. When a button is pressed it will pass an input tag from the playercontroller to here where the abilitysystem can activate/deactivate etc... */
void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) 
	{
		return;
	}

	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive()) 
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		
		}
	}
	

}

/** Part of input system. When a button is pressed it will pass an input tag from the playercontroller to here where the abilitysystem can activate/deactivate etc... */
void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
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


