// Copyright William Marietta


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	SetNetUpdateFrequency(100.f);


	// Create the Ability System Component and Attribute Set
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	/**Sets the replication mode to mixed because there will be multiple player-controlled characters.*/
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);


	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));

}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}


void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	

	DOREPLIFETIME(AAuraPlayerState, Level);

}



void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{

}
