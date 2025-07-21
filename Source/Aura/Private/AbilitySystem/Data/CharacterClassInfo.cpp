// Copyright William Marietta


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaults(ECharacterClass Class) const
{
    return CharacterClassInformation.FindChecked(Class);
}
