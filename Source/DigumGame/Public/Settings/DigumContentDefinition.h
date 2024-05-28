// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumContentDefinition.generated.h"


class UDigumActionBarWidget;
class UDigumInventoryWidget;
// Used for organizing primary game content, dlc, etc.
USTRUCT(BlueprintType)
struct FDigumContentCategory
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName ContentCategoryName;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UDataTable> ItemTable;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UDataTable> AffixTable;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UDataTable> PrefixTable;

	UPROPERTY(EditAnywhere, Category="User Interface")
	TSoftClassPtr<UDigumInventoryWidget> PlayerInventoryWidgetClass;

	UPROPERTY(EditAnywhere, Category="User Interface")
	TSoftClassPtr<UDigumActionBarWidget> PlayerActionBarWidgetClass;
};

/**
 * 
 */
UCLASS()
class DIGUMGAME_API UDigumContentDefinition : public UObject
{
	GENERATED_BODY()
};
