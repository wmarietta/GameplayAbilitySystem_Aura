// Copyright William Marietta


#include "AbilitySystem/ExecCalc/ExecCalcDamage.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include <AbilitySystem/Data/CharacterClassInfo.h>
#include <AbilitySystem/AuraAbilitySystemLibrary.h>
#include <Interaction/CombatInterface.h>
#include <AuraAbilityTypes.h>


struct AuraDamageStatics
{

	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCapture;



	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, MagicResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);


		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, MagicPenetration, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, true);
		
		const FAuraGameplayTags Tags = FAuraGameplayTags::Get();
		TagsToCapture.Add(Tags.Attributes_Secondary_Armor, ArmorDef);
		TagsToCapture.Add(Tags.Attributes_Secondary_MagicResistance, MagicResistanceDef);
		TagsToCapture.Add(Tags.Attributes_Secondary_BlockChance, BlockChanceDef);
		TagsToCapture.Add(Tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCapture.Add(Tags.Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCapture.Add(Tags.Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCapture.Add(Tags.Attributes_Resistance_Lightning, LightningResistanceDef);
		TagsToCapture.Add(Tags.Attributes_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCapture.Add(Tags.Attributes_Resistance_Physical, PhysicalResistanceDef);
		TagsToCapture.Add(Tags.Attributes_Secondary_MagicPenetration, MagicPenetrationDef);

		UE_LOG(LogTemp, Warning, TEXT("AuraDamageStatics Initialized with %d Tags to Capture"), TagsToCapture.Num());
		
	}

};




static const AuraDamageStatics& GetDamageStatics()
{
	static AuraDamageStatics DStatics;
	// Everytime this function is called, attempt to fill out TagToCaptureDef
	if (DStatics.TagsToCapture.Num() < 2)
	{

		const FAuraGameplayTags Tags = FAuraGameplayTags::Get();

		if (Tags.Attributes_Secondary_Armor.IsValid()) {

			DStatics.TagsToCapture.Add(Tags.Attributes_Secondary_Armor, DStatics.ArmorDef);
			DStatics.TagsToCapture.Add(Tags.Attributes_Secondary_MagicResistance, DStatics.MagicResistanceDef);
			DStatics.TagsToCapture.Add(Tags.Attributes_Secondary_BlockChance, DStatics.BlockChanceDef);
			DStatics.TagsToCapture.Add(Tags.Attributes_Secondary_CriticalHitChance, DStatics.CriticalHitChanceDef);
			DStatics.TagsToCapture.Add(Tags.Attributes_Secondary_ArmorPenetration, DStatics.ArmorPenetrationDef);
			DStatics.TagsToCapture.Add(Tags.Attributes_Resistance_Fire, DStatics.FireResistanceDef);
			DStatics.TagsToCapture.Add(Tags.Attributes_Resistance_Lightning, DStatics.LightningResistanceDef);
			DStatics.TagsToCapture.Add(Tags.Attributes_Resistance_Arcane, DStatics.ArcaneResistanceDef);
			DStatics.TagsToCapture.Add(Tags.Attributes_Resistance_Physical, DStatics.PhysicalResistanceDef);
			DStatics.TagsToCapture.Add(Tags.Attributes_Secondary_MagicPenetration, DStatics.MagicPenetrationDef);
		}
	
	}

	return DStatics;
}


UExecCalcDamage::UExecCalcDamage()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().MagicResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalResistanceDef);

	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().MagicPenetrationDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitChanceDef);

	GetDamageStatics();

	
}

void UExecCalcDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

    ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	const FAuraGameplayTags Tags = FAuraGameplayTags::Get();

	//float Damage = 0.f;
	TMap<FGameplayTag, float> DamageByType;
	for (auto& Pair : FAuraGameplayTags::Get().DamageTypesToResistancesMap)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag DamageTypeResistanceTag = Pair.Value;

		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false);
		if (DamageTypeValue > 0.f)
		{
			DamageByType.Add(DamageTypeTag, DamageTypeValue);
		}
	}

	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficientsCurveTable->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetCombatLevel());

	FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficientsCurveTable->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetCombatLevel());

	TMap<FGameplayTag, float> CapturedAttributeValues;
	const float TagsToCaptureSize = GetDamageStatics().TagsToCapture.Num();
	UE_LOG(LogTemp, Warning, TEXT("TagsToCaptureSize: %d"), GetDamageStatics().TagsToCapture.Num());

	for (auto& Pair : GetDamageStatics().TagsToCapture) 
	{
		auto Tag = Pair.Key;
		auto AttributeDef = Pair.Value;

		float CaptureValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributeDef, EvaluationParams, CaptureValue);
		CaptureValue = FMath::Max(0, CaptureValue);

		CapturedAttributeValues.Add(Tag, CaptureValue);
		//UE_LOG(LogTemp, Warning, TEXT("Tag Added: %s, Value Added: %f"), *Tag.ToString(), Value);
		UE_LOG(LogTemp, Warning, TEXT("Tag: %s, Value: %f"), *Tag.ToString(), CaptureValue);
	}

	/*
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().BlockChanceDef, EvaluationParams, TargetBlockChance);
	TargetBlockChance = FMath::Max(0, TargetBlockChance);

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorDef, EvaluationParams, TargetArmor);
	TargetArmor = FMath::Max(0, TargetArmor);


	float TargetFireResist = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().FireResistanceDef, EvaluationParams, TargetFireResist);
	TargetArmor = FMath::Max(0, TargetFireResist);



	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorPenetrationDef, EvaluationParams, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(0, SourceArmorPenetration);


	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitChanceDef, EvaluationParams, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max(0, SourceCriticalHitChance);
	*/

	
	float SourceCriticalHitChance = CapturedAttributeValues[Tags.Attributes_Secondary_CriticalHitChance];
	float TargetBlockChance = CapturedAttributeValues[Tags.Attributes_Secondary_BlockChance];
	float SourceArmorPenetration = CapturedAttributeValues[Tags.Attributes_Secondary_ArmorPenetration];
	float SourceMagicPenetration = CapturedAttributeValues[Tags.Attributes_Secondary_MagicPenetration];


	const bool bIsCriticalHit = FMath::FRandRange(1.f, 100.f) <= SourceCriticalHitChance;
	if (bIsCriticalHit)
	{
		for(auto& DamageType : DamageByType)
		{
			DamageByType[DamageType.Key] *= 2.f;
		}
	}


	const bool bIsBlocked = FMath::FRandRange(1.f, 100.f) <= TargetBlockChance;
	if (bIsBlocked && !bIsCriticalHit)
	{
		for (auto& DamageType : DamageByType)
		{

			DamageByType[DamageType.Key] /= 2.f; 
		}
		
	}

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bIsBlocked);
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bIsCriticalHit);

	

	for (auto& DamageType : DamageByType)
	{
		
		const FGameplayTag DamageTypeTag = DamageType.Key;

		const FGameplayTag PenetrationType = DamageTypeTag == Tags.Damage_Physical ? Tags.Attributes_Secondary_ArmorPenetration : Tags.Attributes_Secondary_MagicPenetration;
		const float Penetration = CapturedAttributeValues[PenetrationType];

		const FGameplayTag ResistanceType = Tags.DamageTypesToResistancesMap[DamageTypeTag];
		const float Resistance = CapturedAttributeValues[ResistanceType];

		const float EffectiveResistance = Resistance * ((100.f - Penetration) / 100.f);

		const float PreDamage = DamageByType[DamageTypeTag];
		DamageByType[DamageTypeTag] *= (100.f - EffectiveResistance) / 100.f;
		const float PostDamage = DamageByType[DamageTypeTag];

		
		UE_LOG(LogTemp, Warning, TEXT(
			"Penetration Type: %s, Penetration: %f, Resistance Type: %s, Resistance: %f, EffectiveResistance: %f, PreMitigatedDamage: %f, PostMitigatedDamage: %f"),
			*PenetrationType.ToString(), Penetration,
			*ResistanceType.ToString(), Resistance,
			EffectiveResistance, PreDamage, PostDamage);
	}
	
	float TotalDamage = 0.f;

	for(auto& DamageType : DamageByType)
	{
		const float Damage = DamageType.Value;
		if (Damage > 0.f)
		{
			TotalDamage += Damage;
		}
	}


	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, TotalDamage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);

}
