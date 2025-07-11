// Copyright William Marietta


#include "AbilitySystem/Data/AttributeInfo.h"

const FAuraAttributeInfo UAttributeInfo::GetAttributeInfo(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	
	if(AttributeMap.Contains(AttributeTag))
	{
		return AttributeMap[AttributeTag];
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attribute %s not found in AttributeInfo %s"), *AttributeTag.ToString(), *GetName());
	}


	return FAuraAttributeInfo();
}
