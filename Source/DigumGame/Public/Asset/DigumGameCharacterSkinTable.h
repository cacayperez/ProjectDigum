// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumGameCharacterSkinTable.generated.h"

class UDigumGameCharacterSkinAsset;

USTRUCT()
struct FDigumGameCharacterSkinTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName SkinID;

	UPROPERTY()
	TSoftClassPtr<UDigumGameCharacterSkinAsset> SkinAsset;
	
};

/**
 * 
 */
UCLASS()
class DIGUMGAME_API UDigumGameCharacterSkinTable : public UObject
{
	GENERATED_BODY()
};
