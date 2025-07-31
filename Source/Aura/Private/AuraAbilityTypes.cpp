#include "AuraAbilityTypes.h"

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	Super::NetSerialize(Ar, Map, bOutSuccess);

	uint32 RepBits = 0;
	
	if (Ar.IsSaving()) 
	{
		if (bIsBlockedHit) 
		{
			RepBits |= 1 << 0; // Bit 0 for bIsBlockedHit
		}

		if (bIsCriticalHit) 
		{
			RepBits |= 1 << 1; // Bit 1 for bIsCriticalHit
		}
	}

	Ar.SerializeBits(&RepBits, 2); // Serialize 2 bits for the flags


	if (RepBits & (1 << 0))
	{
		Ar << bIsBlockedHit;
	}

	if( RepBits & (1 << 1)) 
	{
		Ar << bIsCriticalHit;
	}


	if (Ar.IsLoading()) 
	{
		bIsBlockedHit = (RepBits & (1 << 0)) != 0;
		bIsCriticalHit = (RepBits & (1 << 1)) != 0;
	}



	bOutSuccess = true;
	return true;
}
