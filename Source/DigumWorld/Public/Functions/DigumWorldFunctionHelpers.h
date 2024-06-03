// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumWorldFunctionHelpers.generated.h"

class ADigumWorldActorChild;
struct FDigumWorldBlockTableRow;
class UDigumWorldSwatchAsset;
/**
 * 
 */
UCLASS()
class DIGUMWORLD_API UDigumWorldFunctionHelpers : public UObject
{
	GENERATED_BODY()
public:
	static UDigumWorldSwatchAsset* GetSwatchAsset(const FName& BlockID, const FName& ContentCategory);
	static FDigumWorldBlockTableRow* GetBlockTableRow(const FName& BlockID, UDataTable* InTable);
	static UDataTable* GetSwatchDataTable(const FName ContentCategory);
	static TSubclassOf<ADigumWorldActorChild> GetChildActorClass(const FName& BlockID, const FName& ContentCategory);
};
