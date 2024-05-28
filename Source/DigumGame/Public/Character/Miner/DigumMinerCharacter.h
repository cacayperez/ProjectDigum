// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/DigumCharacter.h"
#include "Interface/IDigumActionComponentInterface.h"
#include "DigumMinerCharacter.generated.h"


struct FDigumGameActionParams;
DECLARE_LOG_CATEGORY_EXTERN(LogDigumMinerCharacter, Log, All);

DECLARE_MULTICAST_DELEGATE(FOnToggleInventory);
DECLARE_MULTICAST_DELEGATE(FOnToggleCharacterMenu);
DECLARE_MULTICAST_DELEGATE(FOnCancelAction);

class USpringArmComponent;
class UCameraComponent;
class UDigumInventoryComponent;
class UDigumGameActionBarComponent;
class UDigumActionComponent;
class UDigumGameEquipComponent;
class UDigumPickupHandlerComponent;
class UDigumGameInventoryComponent;

UENUM()
enum EDigumMinerState : uint8
{
	Default,
	Mining
};

UCLASS()
class DIGUMGAME_API ADigumMinerCharacter : public ADigumCharacter, public IIDigumActionComponentInterface
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Digum Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDigumGameEquipComponent> EquipComponent;

	UPROPERTY(Replicated)
	float FacedDirection = 1.0f;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void Move(const FInputActionValue& InputActionValue);

	FOnToggleInventory OnToggleInventory;
	FOnToggleCharacterMenu OnToggleCharacterMenu;
	FOnCancelAction OnCancelAction;

	UFUNCTION(Server, Reliable)
	void Server_TryActivateEquippedItemAction(const FDigumGameActionParams& InActionParams);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TryActivateEquippedItemAction(const FDigumGameActionParams& InActionParams);

	UFUNCTION(Server, Reliable)
	void Server_SetFaceDirection(float InDirection = -1.0f);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetFaceDirection(float InDirection = -1.0f);

	UFUNCTION(Server, Reliable)
	void Server_EquipItem(const int32& InItemIndex);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EquipItem(const int32& InItemIndex);
	
	virtual void ActivateEquippedItemAction_Internal(const FDigumGameActionParams& ActionParams);
	virtual void EquipItem_Internal(const int32& InItemIndex);
	
public:
	// Sets default values for this character's properties
	ADigumMinerCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void OnActivateEquippedItemAction(const FDigumGameActionParams& InActionParams);

	UFUNCTION()
	void OnActivateInventoryItemAction(const FDigumGameActionParams& InActionParams);

	UFUNCTION()
	void EquipItem(const int32& InItemIndex);
	
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
	void OnInventoryContentChanged();

	UFUNCTION()
	void ActivateEquippedItemAction(const FDigumGameActionParams& InActionParams);
	
public:
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE UDigumGameInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	FORCEINLINE UDigumPickupHandlerComponent* GetPickupHandlerComponent() const { return PickupHandlerComponent; }
	FORCEINLINE UDigumGameActionBarComponent* GetActionBarComponent() const { return ActionBarComponent; }
	FORCEINLINE UDigumGameEquipComponent* GetEquipComponent() const { return EquipComponent; }
	FORCEINLINE FOnToggleInventory& OnToggleInventoryDelegate() { return OnToggleInventory; }
	FORCEINLINE FOnToggleCharacterMenu& OnToggleCharacterMenuDelegate() { return OnToggleCharacterMenu; }
	FORCEINLINE FOnCancelAction& OnCancelActionDelegate() { return OnCancelAction; }

	virtual UDigumActionComponent* GetActionComponent() const override;
	virtual UDigumActionComponent* GetActionComponentBP_Implementation() const override;
	
};
