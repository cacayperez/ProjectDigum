// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "GameState/DigumGameState.h"

#include "GameFramework/PlayerState.h"

ADigumGameState::ADigumGameState()
{
}

TArray<FVector> ADigumGameState::GetAllPlayersLocations() const
{
	if(!HasAuthority())
	{
		return TArray<FVector>();
	}

	TArray<FVector> PlayerLocations;

	for(auto& Player : PlayerArray)
	{
		if(const APawn* PlayerPawn = Player->GetPawn())
		{
			PlayerLocations.Add(PlayerPawn->GetActorLocation());
		}
	}

	return PlayerLocations;
}

FName ADigumGameState::GetWorldContentCategory() const
{
	return WorldContentCategoryName;
}

void ADigumGameState::SetWorldContentCategory(const FName& InContentCategoryName)
{
	WorldContentCategoryName = InContentCategoryName;
}

