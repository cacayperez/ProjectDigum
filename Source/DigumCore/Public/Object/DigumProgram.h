// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumProgram.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMCORE_API UDigumProgram : public UObject
{
	GENERATED_BODY()
public:
	static FString GenerateUniqueID();
	static FString GetProgramVersion();
};
