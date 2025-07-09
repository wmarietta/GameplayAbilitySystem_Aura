// Copyright William Marietta


#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"


UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);

	InitMana(50.f);
	InitMaxMana(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute()) 
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}

}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Properties;
	SetEffectProperties(Data, Properties);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) 
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

}


void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Properties)
{
	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* SourceASC = EffectContext.GetOriginalInstigatorAbilitySystemComponent();

	ACharacter* SourceCharacter = nullptr;
	AActor* SourceAvatarActor = nullptr;
	AController* SourceController = nullptr;

	AActor* TargetAvatarActor = nullptr;
	AController* TargetController = nullptr;
	ACharacter* TargetCharacter = nullptr;
	UAbilitySystemComponent* TargetASC = nullptr;


	if (IsValid(SourceASC) && SourceASC->AbilityActorInfo.IsValid() ) 
	{
		if (SourceASC->AbilityActorInfo->AvatarActor.IsValid()) 
		{
			SourceAvatarActor = SourceASC->AbilityActorInfo->AvatarActor.Get();
			SourceController = SourceASC->AbilityActorInfo->PlayerController.Get();
			if(SourceController == nullptr && SourceAvatarActor != nullptr)
			{
				if(const APawn* Pawn = Cast<APawn>(SourceAvatarActor))
				{
					SourceController = Pawn->GetController();
				}
			}

			if (SourceController) 
			{
				SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
			}
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) 
	{
		TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ACharacter>(TargetAvatarActor);
		TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetAvatarActor);
	}
	

	Properties.EffectContextHandle = EffectContext;

	Properties.SourceAbilitySystemComponent = SourceASC;
	Properties.SourceAvatarActor = SourceAvatarActor;
	Properties.SourceController = SourceController;
	Properties.SourceCharacter = SourceCharacter;

	Properties.TargetAbilitySystemComponent = TargetASC;
	Properties.TargetAvatarActor = TargetAvatarActor;
	Properties.TargetController = TargetController;
	Properties.TargetCharacter = TargetCharacter;

}



void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}




