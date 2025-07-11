// Copyright William Marietta


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"


FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	GameplayTags.Attributes_Secondary_Armor = TagsManager.AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces incoming damage. Improves block chance."));


	/** Primary Attributes */
	GameplayTags.Attributes_Primary_Strength = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"), 
		FString("Increases physical damage."));

	GameplayTags.Attributes_Primary_Agility = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Agility"), 
		FString("Increases critical strike chance and damage."));

	GameplayTags.Attributes_Primary_Intelligence = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"), 
		FString("Increase magical damage, mana, and mana regeneration."));

	GameplayTags.Attributes_Primary_Resilience = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"), 
		FString("Increases armor and magic resistance."));

	GameplayTags.Attributes_Primary_Vigor = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"), 
		FString("Increases health and health regeneration"));


	/** Vital Attributes */
	GameplayTags.Attributes_Vital_Health = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Vital.Health"), 
		FString("Character's total Health"));

	GameplayTags.Attributes_Vital_Mana = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Vital.Mana"), 
		FString("Character's total Mana"));


	/** Secondary Attributes */
	GameplayTags.Attributes_Secondary_MaxHealth = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"), 
		FString("Maximum health of the character."));

	GameplayTags.Attributes_Secondary_MaxMana = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"), 
		FString("Maximum mana of the character."));

	GameplayTags.Attributes_Secondary_Armor = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"), 
		FString("Reduces incoming damage. Improves block chance."));

	GameplayTags.Attributes_Secondary_MagicResistance = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.MagicResistance"), 
		FString("Reduces incoming magical damage."));

	GameplayTags.Attributes_Secondary_ArmorPenetration = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"), 
		FString("Increases damage dealt by ignoring a portion of the target's armor."));

	GameplayTags.Attributes_Secondary_MagicPenetration = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.MagicPenetration"), 
		FString("Increases damage dealt by ignoring a portion of the target's magic resistance."));

	GameplayTags.Attributes_Secondary_CriticalHitChance = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"), 
		FString("Increases the chance to deal a critical hit."));

	GameplayTags.Attributes_Secondary_CriticalHitDamage = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"), 
		FString("Increases the damage dealt by critical hits."));

	GameplayTags.Attributes_Secondary_BlockChance = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"), 
		FString("Increases the chance to block incoming attacks."));

	GameplayTags.Attributes_Secondary_HealthRegeneration = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"), 
		FString("Increases the rate of health regeneration."));

	GameplayTags.Attributes_Secondary_ManaRegeneration = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"), 
		FString("Increases the rate of mana regeneration."));


}
