// Copyright William Marietta

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

struct FUIWidgetRow;

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;


	/**Combat Interface*/
	virtual int32 GetCombatLevel() override;
	/**End Comabat Interface*/


protected:
	virtual void InitAbilityActorInfo() override;

private:

	
	

};
