// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DigumWorldSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Digum World Settings") )
class DIGUMWORLD_API UDigumWorldSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(Config, EditAnywhere, Category = "World", meta=(DisplayName="Grid Size"))
	FVector GridSize = FVector(160.0f, 200.0f, 160.0f);
	
};
