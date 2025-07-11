// Copyright William Marietta

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 *	AuraGameplayTags 
 * 
 * Singleton containing native GameplayTags for the Aura project.
 */


struct FAuraGameplayTags {
	
public:
	static const FAuraGameplayTags& Get() 
	{
		return GameplayTags;
	}

	static void InitializeNativeGameplayTags();


	
	/**Primary Attribute Tags*/
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Agility;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;


	/**Vital Attribute Tags*/
	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Mana;


	/**Secondary Attribute Tags*/
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_MagicResistance;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_MagicPenetration;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;


	/** Message Tags */
	





protected:

private:
	static FAuraGameplayTags GameplayTags;


};

