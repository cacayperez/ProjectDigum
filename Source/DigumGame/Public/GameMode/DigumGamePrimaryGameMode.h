// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DigumGamePrimaryGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMGAME_API ADigumGamePrimaryGameMode : public AGameMode
{
	GENERATED_BODY()
	// We will only use positive seeds
	// Throw error if is on left side of number plane
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Digum Game State", meta=(AllowPrivateAccess="true"))
	int32 WorldSeed = -1;


public:
	ADigumGamePrimaryGameMode();

	int32 GetWorldSeed() const;
	void SetWorldSeed(const int32& InValue);


	virtual void Tick(float DeltaSeconds) override;
};
