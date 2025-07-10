// Copyright William Marietta

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"

#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:

	/** Default Constructor for AuraEnemy */
	AAuraEnemy();


	//~ Enemy Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End Enemy Interface


	virtual int32 GetCombatLevel() override;


protected:

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

private:

	
	
};
