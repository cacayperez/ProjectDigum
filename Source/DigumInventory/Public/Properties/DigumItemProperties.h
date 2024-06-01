// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumItemProperties.generated.h"

USTRUCT(BlueprintType)
struct FDigumItemAffixProperties
{
	GENERATED_BODY()
public:
	FName AffixID;
	int32 AffixLevel;

	
};

/**
 * Base struct for item properties
 * that are stored in a save file, or sent thru network
 */
USTRUCT(BlueprintType)
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Inventory")
	FName ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Inventory")
	int32 ItemAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Inventory")
	TArray<FDigumItemAffixProperties> ItemPrefixes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Inventory")
	TArray<FDigumItemAffixProperties> ItemSuffixes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Inventory")
	FName ContentCategory;

	FORCEINLINE bool IsValid() const
	{
		return ItemID != NAME_None;
	}

	FORCEINLINE FName GetItemID() const
	{
		return ItemID;
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

	FORCEINLINE FName GetContentCategory() const { return ContentCategory; }
};

