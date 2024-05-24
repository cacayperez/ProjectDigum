// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumActionProperties.generated.h"

class UDigumAction;

USTRUCT()
struct FDigumActionProperties
{
	GENERATED_BODY()
public:
	UPROPERTY()
	AActor* ActionInstigator = nullptr;

	UPROPERTY()
	UObject* Payload = nullptr;
	
	UPROPERTY()
	TSubclassOf<UDigumAction> ActionClass;
};
