// Copyright William Marietta

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"


class UGameplayEffect;


UENUM(BlueprintType)
enum class ECharacterClass : uint8 
{
	Elementalist UMETA(DisplayName = "Elementalist"),
	Warrior UMETA(DisplayName = "Warrior"),
	Ranger UMETA(DisplayName = "Ranger")
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes; 

};


/**
 * 
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;


	FCharacterClassDefaultInfo GetClassDefaults(ECharacterClass Class) const;

};
