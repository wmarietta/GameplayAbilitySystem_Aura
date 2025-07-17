// Copyright William Marietta


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* Task = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return Task;
}

void UTargetDataUnderMouse::Activate()
{

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}

	else 
	{
		//TODO: We are on the server, so listen for the target data from the client
	}

	
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	
	FScopedPredictionWindow PredictionWindow(AbilitySystemComponent.Get());

	
	FHitResult HitResult;
	Ability->GetCurrentActorInfo()->PlayerController.Get()->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	

	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit;
	TargetData->HitResult = HitResult;

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(TargetData);

	FGameplayTag ApplicationTag;
	


	
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(), 
		GetActivationPredictionKey(), 
		TargetDataHandle, 
		FGameplayTag(), 
		AbilitySystemComponent->ScopedPredictionKey);


	if (ShouldBroadcastAbilityTaskDelegates()) 
	{
		OnValidData.Broadcast(TargetData);
	}

}
