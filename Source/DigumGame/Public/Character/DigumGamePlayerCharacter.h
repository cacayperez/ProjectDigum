// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interface/IDigumPlayerCharacterInterface.h"
#include "Interface/IDigumWorldPawnInterface.h"
#include "Miner/DigumMinerCharacter.h"
#include "DigumGamePlayerCharacter.generated.h"

class ADigumMinerPlayerController;

UCLASS()
class DIGUMGAME_API ADigumGamePlayerCharacter : public ADigumMinerCharacter, public IIDigumPlayerCharacterInterface
{
	GENERATED_BODY()

	
public:
	// Sets default values for this character's properties
	ADigumGamePlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
public:
	virtual APlayerController* GetPlayerController() const override;
	virtual float GetVisibilityRadius() const override;

	// virtual void InitializeInputBindings(ADigumMinerPlayerController* PlayerController);
	virtual void PossessedBy(AController* NewController) override;
	
};
