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
	static void CreateSaveCharacter(UDigumGameSavedGameObject_Character* InSavedGameObject, const FString& InSaveSlotName, int32 InUserIndex);
	static FString GetFullFolderPath(FString InLocalPath);
	
	
};
