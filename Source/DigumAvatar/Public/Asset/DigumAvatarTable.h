// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumAvatarTable.generated.h"


class UDigumAvatarAsset;

USTRUCT()
struct FDigumAvatarTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FName AvatarID;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UDigumAvatarAsset> AvatarAsset;
};


/**
 * 
 */
UCLASS()
class DIGUMAVATAR_API UDigumAvatarTable : public UObject
{
	GENERATED_BODY()
};
