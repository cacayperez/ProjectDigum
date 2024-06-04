// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DigumPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMCORE_API ADigumPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	UPROPERTY()
	FText PlayerName;

public:
	FVector GetPlayerLocation() const;
};
