// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interface/IDigumPlayerCharacterInterface.h"
#include "Miner/DigumMinerCharacter.h"
#include "DigumGamePlayerCharacter.generated.h"

UCLASS()
class DIGUMGAME_API ADigumGamePlayerCharacter : public ADigumMinerCharacter, public IIDigumPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADigumGamePlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual APlayerController* GetPlayerController() const override;
	virtual float GetVisibilityRadius() const override;
	
};
