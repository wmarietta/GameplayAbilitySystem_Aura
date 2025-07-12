// Copyright William Marietta


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"


void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair : AS->TagToAttributeDelegateMap) 
	{
		FAuraAttributeInfo Info = AttributeInfo->GetAttributeInfo(Pair.Key);
		Info.AttributeValue = Pair.Value.Execute().GetNumericValue(AS);

		AttributeInfoDelegate.Broadcast(Info);
	}


}

void UAttributeMenuWidgetController::BindCallbacksToDependencies() 
{
	Super::BindCallbacksToDependencies();


	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	for (auto& Pair : AS->TagToAttributeDelegateMap)
	{
		FGameplayAttribute Attribute = Pair.Value.Execute();

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda(
			[this, Pair, AS](const FOnAttributeChangeData& Data)
			{
				FAuraAttributeInfo Info = AttributeInfo->GetAttributeInfo(Pair.Key);
				Info.AttributeValue = Pair.Value.Execute().GetNumericValue(AS);

				AttributeInfoDelegate.Broadcast(Info);
			
			}
		);

		
	}

}



