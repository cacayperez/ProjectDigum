// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumGameSaveFunctionHelper.generated.h"

class UDigumGameSavedGameObject_Character;
/**
 * 
 */
UCLASS()
class DIGUMGAME_API UDigumGameSaveFunctionHelper : public UObject
{
	GENERATED_BODY()
public:
	static void CreateSaveCharacter(UDigumGameSavedGameObject_Character* SavedGameObject, FString SaveSlotName, int32 UserIndex);
	
	
};
