// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumWorldTypes.generated.h"

USTRUCT()
struct FDigumWorldCoordinate2D
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	int32 X = -1;

	UPROPERTY(VisibleAnywhere)
	int32 Y = -1;

	bool operator==(const FDigumWorldCoordinate2D& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	bool IsValid() const { return X != -1 && Y != -1; }
};

/**
 * 
 */
UCLASS()
class DIGUMCORE_API UDigumWorldTypes : public UObject
{
	GENERATED_BODY()
};
