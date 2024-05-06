// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/DigumCharacter.h"
#include "DigumMinerCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDigumMinerCharacter, Log, All);

DECLARE_MULTICAST_DELEGATE(FOnToggleInventory);
DECLARE_MULTICAST_DELEGATE(FOnToggleCharacterMenu);
DECLARE_MULTICAST_DELEGATE(FOnCancelAction);

class USpringArmComponent;
class UCameraComponent;
class UDigumInventoryComponent;

UCLASS()
class DIGUMGAME_API ADigumMinerCharacter : public ADigumCharacter
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Digum Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Digum Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Digum Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDigumInventoryComponent> InventoryComponent;

protected:
	void Move(const FInputActionValue& InputActionValue);

	FOnToggleInventory OnToggleInventory;
	FOnToggleCharacterMenu OnToggleCharacterMenu;
	FOnCancelAction OnCancelAction;
	
public:
	// Sets default values for this character's properties
	ADigumMinerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	

public:
	virtual void PrimaryAction();
	virtual void SecondaryAction();
	virtual void ToggleInventory();
	virtual void ToggleCharacterMenu();
	virtual void CancelAction();
	
public:
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE UDigumInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	FORCEINLINE FOnToggleInventory& OnToggleInventoryDelegate() { return OnToggleInventory; }
	FORCEINLINE FOnToggleCharacterMenu& OnToggleCharacterMenuDelegate() { return OnToggleCharacterMenu; }
};
