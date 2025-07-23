// Copyright William Marietta


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Aura/Aura.h"


AAuraCharacterBase::AAuraCharacterBase()
{

	PrimaryActorTick.bCanEverTick = false;


	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);



	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}


void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo() 
{

}

FVector AAuraCharacterBase::GetCombatSocketLocation() const
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}



void AAuraCharacterBase::InitializeDefaultAttributes()
{
	ApplyEffectToSelf(DefaultPrimaryAttributeEffectClass, 1.0f);
	ApplyEffectToSelf(DefaultSecondaryAttributeEffectClass, 1.0f);
	ApplyEffectToSelf(DefaultVitalAttributeEffectClass, 1.0f);
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> AttributeEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(AttributeEffectClass);


	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(AttributeEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority()) 
	{
		return;
	}

	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());
	AuraASC->AddCharacterAbilities(StartingAbilities);

}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}




