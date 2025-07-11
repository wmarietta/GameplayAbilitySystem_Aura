// Copyright William Marietta

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*Asset Tags*/);



/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()


public:
	void RegisterToAbilitySystemDelegates();

	FEffectAssetTags OnEffectAssetTagsAppliedDelegate;


	void AddCharacterAbilities(const TArray<TSubclassOf<class UGameplayAbility>>& AbilitiesToAdd);

protected:


	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, const FActiveGameplayEffectHandle ActiveEffectHandle);


	UFUNCTION(Client, Reliable)
	void Client_EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, const FActiveGameplayEffectHandle ActiveEffectHandle);

private:

	void BroadcastEffectAssetTags(const FGameplayEffectSpec& EffectSpec);
};
