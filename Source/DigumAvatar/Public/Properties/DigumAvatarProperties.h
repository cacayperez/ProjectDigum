// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumAvatarProperties.generated.h"

USTRUCT(BlueprintType)
struct FDigumAvatarProperties
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Avatar")
	FName AvatarID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Avatar")
	FName ContentCategory;

	
};
