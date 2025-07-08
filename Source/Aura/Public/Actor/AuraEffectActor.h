// Copyright William Marietta

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"

#include "AuraEffectActor.generated.h"


class UGameplayEffect;
class UAbilitySystemComponent;


UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8 {
	ApplyOnOverlap UMETA(DisplayName = "Apply on Overlap"),
	ApplyOnEndOverlap UMETA(DisplayName = "Apply on End Overlap"),
	DoNotApply UMETA(DisplayName = "Do Not Apply")
};


UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8 {
	RemoveOnOverlap UMETA(DisplayName = "Apply on Overlap"),
	RemoveOnEndOverlap UMETA(DisplayName = "Apply on End Overlap"),
	DoNotRemove UMETA(DisplayName = "Do Not Apply")
};


UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AAuraEffectActor();

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Effect")
	bool bDestroyOnEffectRemove = false;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Effect")
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Effect")
	EEffectRemovalPolicy EffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;


	UFUNCTION(BlueprintCallable, Category = "Aura|Effect")
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectClass);


	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Aura|Effect")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;


	TMap<UAbilitySystemComponent* ,FActiveGameplayEffectHandle> ActiveEffectsMap;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties")
	float ActorLevel = 1.f;



private:


};
