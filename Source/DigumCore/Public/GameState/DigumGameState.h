// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DigumGameState.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMCORE_API ADigumGameState : public AGameState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Digum Game State", meta=(AllowPrivateAccess="true"))
	FName WorldContentCategoryName = TEXT("Primary");
	
public:
	ADigumGameState();

	TArray<FVector> GetAllPlayersLocations() const;

	FName GetWorldContentCategory() const;
	void SetWorldContentCategory(const FName& InContentCategoryName);
};
