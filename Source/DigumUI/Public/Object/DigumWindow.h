// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumWidget.h"
#include "UObject/Object.h"
#include "Window/SDigumWindow.h"
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
	bool bShowHeader = true;

	UPROPERTY(EditAnywhere)
	bool bEnableWindowDrag = true;
	
	UPROPERTY(EditAnywhere)
	FDigumFontSettings HeaderFontSettings;

	UPROPERTY(EditAnywhere)
	FDigumFontSettings ContentFontSettings;
	
	bool ShouldShowHeader() const { return bShowHeader; }
	bool IsDraggable() const { return bEnableWindowDrag; }
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
