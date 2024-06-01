// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumGameItemHelperFunctions.generated.h"

class UDigumGameItemAsset;
/**
 * 
 */
UCLASS()
class DIGUMGAME_API UDigumGameItemHelperFunctions : public UObject
{
	GENERATED_BODY()
public:
	static UDigumGameItemAsset* GetGameItemAsset(const FName& ItemID, const FName& ContentCategory);
	static void GetCursorToWorldLocation(const APlayerController* PlayerController, FVector& OutWorldLocation, FVector& OutWorldDirection);
};
