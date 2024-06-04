// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DigumSaveSubystem.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMSAVE_API UDigumSaveSubystem : public UWorldSubsystem
{
	GENERATED_BODY()


public:
	UDigumSaveSubystem();

	static UDigumSaveSubystem* Get(const UWorld* InWorld);

};
