// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumWorldTileDefinition.generated.h"

USTRUCT(BlueprintType)
struct FDigumWorldTileInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum World Tile")
	UStaticMesh* TileMesh;
};
