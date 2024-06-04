// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Player/DigumPlayerState.h"

FVector ADigumPlayerState::GetPlayerLocation() const
{
	if(GetPawn())
	{
		return GetPawn()->GetActorLocation();
	}

	return FVector::ZeroVector;
}

