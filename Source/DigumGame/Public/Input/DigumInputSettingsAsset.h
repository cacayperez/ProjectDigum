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

	UPROPERTY(EditAnywhere, Category = "Input", meta=(DisplayName="Cancel Action"))
	TObjectPtr<UInputAction> CancelAction;
	
	UPROPERTY(EditAnywhere, Category = "Input", meta=(DisplayName="Primary Right Trigger Action"))
	TObjectPtr<UInputAction> PrimaryRightTriggerAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(DisplayName="Primary Left Trigger Action"))
	TObjectPtr<UInputAction> PrimaryLeftTriggerAction;

	// Context Action for viewing inventory, etc.
	UPROPERTY(EditAnywhere, Category = "Input", meta=(DisplayName="Context Action 1"))
	TObjectPtr<UInputAction> CharacterContextAction1;

	// Context Action for viewing other advanced information, etc.
	UPROPERTY(EditAnywhere, Category = "Input", meta=(DisplayName="Context Action 2"))
	TObjectPtr<UInputAction> CharacterContextAction2;
	
};
