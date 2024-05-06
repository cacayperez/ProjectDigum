// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumItemTable.generated.h"

class UDigumItemAsset;

USTRUCT()
struct FDigumItemTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName ItemID;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UDigumItemAsset> ItemAsset;
};

/**
 * 
 */
UCLASS()
class DIGUMINVENTORY_API UDigumItemTable : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Digum Inventory", meta=(DisplayName="Get Digum Item Asset"))
	static UDigumItemAsset* GetDigumItemAsset(const FName& InItemID, UDataTable* InItemTable);

	UFUNCTION(BlueprintCallable, Category = "Digum Inventory", meta=(DisplayName="Get Digum Item StackSize"))
	static int32 GetDigumItemStackSize(const FName& InItemID, UDataTable* InItemTable);
	
private:
	static bool GetItemTableRow(const FName& InItemID, UDataTable* InItemTable, FDigumItemTableRow& OutItemTableRow);
};
