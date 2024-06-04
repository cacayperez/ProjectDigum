// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/DigumGamePlayerCharacter.h"


ADigumGamePlayerCharacter::ADigumGamePlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

APlayerController* ADigumGamePlayerCharacter::GetPlayerController() const
{
	if(GetController())
	{
		return Cast<APlayerController>(GetController());
	}
	return nullptr;
}

float ADigumGamePlayerCharacter::GetVisibilityRadius() const
{
	return 3000.0f;
}
