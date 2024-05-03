// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumWorld.generated.h"

USTRUCT()
struct FDigumWorldSize
{
	GENERATED_BODY()
public:
	FDigumWorldSize(){}
	FDigumWorldSize(const int32& InWidth, const int32& InHeight)
		: Width(InWidth)
		, Height(InHeight)
	{
	}

	UPROPERTY(EditAnywhere)
	int32 Width = 0;

	UPROPERTY(EditAnywhere)
	int32 Height = 0;

	FORCEINLINE void operator=(const FDigumWorldSize& Other)
	{
		Width = Other.Width;
		Height = Other.Height;
	}

	FORCEINLINE bool operator==(const FDigumWorldSize& Other) const
	{
		return Width == Other.Width && Height == Other.Height;
	}
};

USTRUCT()
struct FDigumWorldCoordinate
{
	GENERATED_BODY()
public:
	FDigumWorldCoordinate(){}
	FDigumWorldCoordinate(const int32& InX, const int32& InY)
		: X(InX)
		, Y(InY)
	{
	}
	UPROPERTY(EditAnywhere)
	int32 X = 0;

	UPROPERTY(EditAnywhere)
	int32 Y = 0;

	FORCEINLINE void operator=(const FDigumWorldCoordinate& Other)
	{
		X = Other.X;
		Y = Other.Y;
	}

	FORCEINLINE bool operator==(const FDigumWorldCoordinate& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}
};

/**
 * Not a child of UWorld, but a separate UObject that holds the settings for the DigumWorld subsystem.
 */
UCLASS()
class DIGUMWORLD_API UDigumWorld : public UObject
{
	GENERATED_BODY()
};
