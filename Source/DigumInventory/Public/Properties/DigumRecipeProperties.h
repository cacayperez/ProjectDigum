// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumRecipeProperties.generated.h"

USTRUCT()
struct FDigumIngredientProperties
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName ItemID;

	UPROPERTY()
	int32 ItemAmount;
	
};

USTRUCT()
struct FDigumRecipeProperties
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName RecipeID;
	
	UPROPERTY()
	TArray<FDigumIngredientProperties> Ingredients;
	
};
