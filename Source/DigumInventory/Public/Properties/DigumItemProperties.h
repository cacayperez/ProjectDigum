// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumItemProperties.generated.h"

USTRUCT()
struct FDigumItemAffixProperties
{
	GENERATED_BODY()
public:
	FName AffixID;
	int32 AffixLevel;

	
};

USTRUCT()
struct FDigumItemProperties
{
	GENERATED_BODY()
public:
	FDigumItemProperties() : ItemID(NAME_None), ItemAmount(0) {}
	FDigumItemProperties(const FName& InItemID, const int32 InAmount = 1)
	{
		ItemID = InItemID;
		ItemAmount = InAmount;
	}
	
	FName ItemID;
	int32 ItemAmount;
	TArray<FDigumItemAffixProperties> ItemPrefixes;
	TArray<FDigumItemAffixProperties> ItemSuffixes;

	FORCEINLINE bool IsValid() const
	{
		return ItemID != NAME_None;
	}

	FORCEINLINE void AddPrefix(const FDigumItemAffixProperties& InPrefix)
	{
		ItemPrefixes.Add(InPrefix);
	}

	FORCEINLINE void AddSuffix(const FDigumItemAffixProperties& InSuffix)
	{
		ItemSuffixes.Add(InSuffix);
	}

	FORCEINLINE int32 GetAmount() const
	{
		return ItemAmount;
	}
};

