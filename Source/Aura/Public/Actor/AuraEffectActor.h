// Copyright William Marietta

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AAuraEffectActor();

protected:
	
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable, Category = "Aura|Effect")
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectClass);


	UPROPERTY(EditAnywhere, Category = "Aura|Effect")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;




private:


};
