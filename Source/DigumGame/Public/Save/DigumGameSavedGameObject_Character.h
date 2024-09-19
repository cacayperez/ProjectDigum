// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumSavedGameObject.h"
#include "UObject/Object.h"
#include "DigumGameSavedGameObject_Character.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMGAME_API UDigumGameSavedGameObject_Character : public UDigumSavedGameObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString CharacterName;

	UPROPERTY()
	FName BaseCharacterSkinID = NAME_None;

	UPROPERTY()
	FName CharacterClassID = NAME_Name;

	UPROPERTY()
	int32 DifficultySetting = 0;

	
	
};
