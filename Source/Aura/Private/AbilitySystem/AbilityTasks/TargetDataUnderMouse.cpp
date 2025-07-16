// Copyright William Marietta


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* Task = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return Task;
}

void UTargetDataUnderMouse::Activate()
{

	FHitResult HitResult;
	Ability->GetCurrentActorInfo()->PlayerController.Get()->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	OnValidData.Broadcast(HitResult.ImpactPoint);
}
