// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/DigumCharacter.h"
#include "DigumMinerCharacter.generated.h"

class UDigumPickupHandlerComponent;
class UDigumGameInventoryComponent;
DECLARE_LOG_CATEGORY_EXTERN(LogDigumMinerCharacter, Log, All);

DECLARE_MULTICAST_DELEGATE(FOnToggleInventory);
DECLARE_MULTICAST_DELEGATE(FOnToggleCharacterMenu);
DECLARE_MULTICAST_DELEGATE(FOnCancelAction);

class USpringArmComponent;
class UCameraComponent;
class UDigumInventoryComponent;
class UDigumGameActionBarComponent;
class UDigumActionComponent;

UENUM()
enum EDigumMinerState : uint8
{
	Default,
	Mining
};

UCLASS()
class DIGUMGAME_API ADigumMinerCharacter : public ADigumCharacter
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Digum Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Digum Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Digum Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDigumGameInventoryComponent> InventoryComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Digum Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDigumPickupHandlerComponent> PickupHandlerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Digum Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDigumGameActionBarComponent> ActionBarComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Digum Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDigumActionComponent> ActionComponent;

	UPROPERTY(Replicated)
	float FacedDirection = 1.0f;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void Move(const FInputActionValue& InputActionValue);

	FOnToggleInventory OnToggleInventory;
	FOnToggleCharacterMenu OnToggleCharacterMenu;
	FOnCancelAction OnCancelAction;

	UFUNCTION(Server, Reliable)
	void Server_TryActivateAction(const int32& InItemIndex);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TryActivateAction(const int32& InItemIndex);

	UFUNCTION(Server, Reliable)
	void Server_SetFaceDirection(float InDirection = -1.0f);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetFaceDirection(float InDirection = -1.0f);
	
	
	virtual void ActivateAction_Internal(const int32& InItemIndex);
public:
	// Sets default values for this character's properties
	ADigumMinerCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void OnActivateItemAction(const int32& InItemIndex);
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:
	virtual void SetFaceDirection(float InDirection = 1.0f);
	virtual void PrimaryAction();
	virtual void SecondaryAction();
	virtual void ToggleInventory();
	virtual void ToggleCharacterMenu();
	virtual void CancelAction();
	virtual void SelectActionBar_0();
	virtual void SelectActionBar_1();
	virtual void SelectActionBar_2();
	virtual void SelectActionBar_3();
	virtual void SelectActionBar_4();

	void UpdateMeshScale();
public:
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE UDigumGameInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	FORCEINLINE UDigumPickupHandlerComponent* GetPickupHandlerComponent() const { return PickupHandlerComponent; }
	FORCEINLINE UDigumGameActionBarComponent* GetActionBarComponent() const { return ActionBarComponent; }
	FORCEINLINE UDigumActionComponent* GetActionComponent() const { return ActionComponent; }
	FORCEINLINE FOnToggleInventory& OnToggleInventoryDelegate() { return OnToggleInventory; }
	FORCEINLINE FOnToggleCharacterMenu& OnToggleCharacterMenuDelegate() { return OnToggleCharacterMenu; }
	FORCEINLINE FOnCancelAction& OnCancelActionDelegate() { return OnCancelAction; }
};
