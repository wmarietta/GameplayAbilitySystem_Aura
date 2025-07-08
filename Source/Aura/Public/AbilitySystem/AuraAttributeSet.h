// Copyright William Marietta

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "AuraAttributeSet.generated.h"



#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)



USTRUCT(BlueprintType)
struct FEffectProperties {

	GENERATED_BODY()

	FEffectProperties() {}



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties")
	FGameplayEffectContextHandle EffectContextHandle;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties")
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties")
	TObjectPtr<AActor> SourceAvatarActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties")
	TObjectPtr<AController> SourceController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties")
	TObjectPtr<ACharacter> SourceCharacter;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties")
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties")
	TObjectPtr<AActor> TargetAvatarActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties")
	TObjectPtr<AController> TargetController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties")
	TObjectPtr<ACharacter> TargetCharacter;


	


};



/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	/** Default Constructor for AuraAttributeSets*/
	UAuraAttributeSet();

	/** Override the GetLifetimeReplicatedProps function to replicate attributes */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/** The character's current health */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Main Attributes")
	FGameplayAttributeData Health;

	/** The character's maximum health */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Main Attributes")
	FGameplayAttributeData MaxHealth;

	/** The character's current mana */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Main Attributes")
	FGameplayAttributeData Mana;

	/** The character's maximum mana */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Main Attributes")
	FGameplayAttributeData MaxMana;





	//~Attribute Accessors
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);
	//~End Attribute Accessors


	//~Replicate Functions for Attributes

	/** Attribute change notification for AuraAttributeSet
	 * The functions are called when the attribute changes
	 * @param Old<AttributeName> The old value
	*/

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	//~End Replicate Functions for Attributes
	
private:

	void SetEffectProperties(const FGameplayEffectModCallbackData &Data, FEffectProperties& Properties );
	
};
