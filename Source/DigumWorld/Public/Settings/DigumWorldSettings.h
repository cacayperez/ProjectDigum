// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DigumWorldMapActor.h"
#include "Engine/DeveloperSettings.h"
#include "Procedural/DigumWorldGenerator.h"
#include "DigumWorldSettings.generated.h"

USTRUCT(BlueprintType)
struct FDigumWorldContentCategory
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName ContentCategoryName;

	UPROPERTY(EditAnywhere, Category = "World", meta=(DisplayName="Procedural Rules"))
	FDigumWorldProceduralRules ProceduralRules;

	UPROPERTY(EditAnywhere, Category = "World", meta=(DisplayName="Table"))
	UDataTable* BlockTable;
};
/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Digum World Settings") )
class DIGUMWORLD_API UDigumWorldSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(Config, EditAnywhere, Category = "World", meta=(DisplayName="Grid Size"))
	FVector GridSize = FVector(160.0f, 200.0f, 160.0f);

	UPROPERTY(Config, EditAnywhere, Category = "Content", meta=(DisplayName="World Content Category"))
	TArray<FDigumWorldContentCategory> WorldContentCategories;

	UPROPERTY(Config, EditAnywhere, Category = "Content", meta=(DisplayName="World Content Category"))
	TSoftClassPtr<ADigumWorldMapActor> WorldMapActorClass;
	
	static const UDigumWorldSettings* Get();
	static const FDigumWorldContentCategory* GetWorldContentCategoryData(FName ContentCategoryName);
	static FVector GetGridSize();
	static TSubclassOf<ADigumWorldMapActor> GetWorldMapActorClass();
};
