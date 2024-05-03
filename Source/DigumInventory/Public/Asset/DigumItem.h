// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumItem.generated.h"






/**
 * 
 */
UCLASS()
class DIGUMINVENTORY_API UDigumItem : public UObject
{
	GENERATED_BODY()
public:
	FName ItemID;
	int32 ItemAmount;
};
