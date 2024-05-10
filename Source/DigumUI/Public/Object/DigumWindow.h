// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumWidget.h"
#include "UObject/Object.h"
#include "DigumWindow.generated.h"

USTRUCT()
struct FDigumFontSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	UFont* Font = nullptr;

	UPROPERTY(EditAnywhere)
	float FontSize = 10;

	UPROPERTY(EditAnywhere)
	FSlateColor Color;
};

USTRUCT()
struct FDigumWindowStyle
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FText TitleName;

	UPROPERTY(EditAnywhere)
	FDigumFontSettings HeaderFontSettings;

	UPROPERTY(EditAnywhere)
	FDigumFontSettings ContentFontSettings;
};
/**
 * 
 */
UCLASS()
class DIGUMUI_API UDigumWindow : public UDigumWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Digum Window")
	FDigumWindowStyle WindowStyle;
};
