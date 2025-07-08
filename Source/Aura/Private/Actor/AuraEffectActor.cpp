// Copyright William Marietta


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


AAuraEffectActor::AAuraEffectActor()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")));

}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) 
	{
		ApplyEffectToTarget(TargetActor, GameplayEffectClass);
	}

	if (EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnOverlap) 
	{
		
	}

}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) 
	{
		ApplyEffectToTarget(TargetActor, GameplayEffectClass);
	}

	if(EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) 
	{
		UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

		if (!IsValid(TargetAbilitySystemComponent)) 
		{
			return;
		}


		if (ActiveEffectsMap.Contains(TargetAbilitySystemComponent)) 
		{
			FActiveGameplayEffectHandle ActiveEffectHandle = ActiveEffectsMap[TargetAbilitySystemComponent];
			if (ActiveEffectHandle.IsValid()) 
			{
				TargetAbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffectHandle, 1);
				ActiveEffectsMap.Remove(TargetAbilitySystemComponent);
			}
		}
		
	}
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectClass)
{
	TScriptInterface<IAbilitySystemInterface> AbilitySystemInterface = TargetActor;
	if (AbilitySystemInterface == nullptr) 
	{
		return;
	}

	check(EffectClass);

	
	UAbilitySystemComponent* TargetAbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();

	FGameplayEffectContextHandle EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(EffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && EffectRemovalPolicy != EEffectRemovalPolicy::DoNotRemove) 
	{
		ActiveEffectsMap.Add(TargetAbilitySystemComponent, ActiveEffectHandle);
	}
	
}



