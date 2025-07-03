// Copyright William Marietta

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"


#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * This class represents the player state in the Aura game.
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()


public:
	/** Default constructor for the AuraPlayerState class. Initializes the player state. */
	AAuraPlayerState();
	
	/**Returns the Player's UAbilitySystemComponent*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/**Returns the Player's UAttributeSet*/
	UAttributeSet* GetAttributeSet() const;


protected:

	/**The Player's Ability System Component*/
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;


	/**The Player's Attribute Set*/
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
