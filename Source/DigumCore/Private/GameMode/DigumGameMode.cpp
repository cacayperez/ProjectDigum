// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "GameMode/DigumGameMode.h"

ADigumGameMode::ADigumGameMode()
{
}

FName ADigumGameMode::GetWorldContentCategory() const
{
	return WorldContentCategoryName;
}

void ADigumGameMode::SetWorldContentCategory(const FName& InContentCategoryName)
{
	if(InContentCategoryName != NAME_None)
		WorldContentCategoryName = InContentCategoryName;
}
