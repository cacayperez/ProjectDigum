// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DigumInputSettingsAsset.generated.h"

class UInputAction;
class UInputMappingContext;


/**
 * 
 */
UCLASS()
class DIGUMGAME_API UDigumInputSettingsAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Input", meta=(DisplayName="Mapping Context"))
	TObjectPtr<UInputMappingContext> MappingContext;
	
	UPROPERTY(EditAnywhere, Category = "Input", meta=(DisplayName="Move Action"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(DisplayName="Jump Action"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(DisplayName="Primary Action"))
	TObjectPtr<UInputAction> PrimaryAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(DisplayName="Secondary Action"))
	TObjectPtr<UInputAction> SecondaryAction;

	
};
