// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/DigumPlayerController.h"
#include "DigumMinerPlayerController.generated.h"

struct FInputActionValue;

UCLASS()
class DIGUMGAME_API ADigumMinerPlayerController : public ADigumPlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADigumMinerPlayerController();

protected:

	/*
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerInput_Move, const FInputActionValue&, InputActionValue);
	DECLARE_MULTICAST_DELEGATE(FOnControllerInput_Jump);
	DECLARE_MULTICAST_DELEGATE(FOnControllerInput_PrimaryAction);
	DECLARE_MULTICAST_DELEGATE(FOnControllerInput_SecondaryAction);
	DECLARE_MULTICAST_DELEGATE(FOnControllerInput_ToggleInventory);
	DECLARE_MULTICAST_DELEGATE(FOnControllerInput_ToggleCharacterMenu);
	DECLARE_MULTICAST_DELEGATE(FOnControllerInput_CancelAction);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnControllerInput_SelectActionBar, const int32&);
	
	FOnControllerInput_Move OnControllerInput_Move;
	FOnControllerInput_Jump OnControllerInput_Jump;
	FOnControllerInput_PrimaryAction OnControllerInput_PrimaryAction;
	FOnControllerInput_SecondaryAction OnControllerInput_SecondaryAction;
	FOnControllerInput_ToggleInventory OnControllerInput_ToggleInventory;
	FOnControllerInput_ToggleCharacterMenu OnControllerInput_ToggleCharacterMenu;
	FOnControllerInput_CancelAction	OnControllerInput_CancelAction;
	FOnControllerInput_SelectActionBar OnControllerInput_SelectActionBar;*/
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// void ClearInputBindings();
	virtual void OnPossess(APawn* InPawn) override;
	

public:
	/*UFUNCTION()
	void Move(const FInputActionValue& InputActionValue);
	void Jump();
	virtual void PrimaryAction();
	virtual void SecondaryAction();
	virtual void ToggleInventory();
	virtual void ToggleCharacterMenu();
	virtual void CancelAction();
	virtual void SelectActionBar_0();
	virtual void SelectActionBar_1();
	virtual void SelectActionBar_2();
	virtual void SelectActionBar_3();
	virtual void SelectActionBar_4();*/


	/*
	FOnControllerInput_Move& GetOnControllerInput_Move() { return OnControllerInput_Move; }
	FOnControllerInput_Jump& GetOnControllerInput_Jump() { return OnControllerInput_Jump; }
	FOnControllerInput_PrimaryAction& GetOnControllerInput_PrimaryAction() { return OnControllerInput_PrimaryAction; }
	FOnControllerInput_SecondaryAction& GetOnControllerInput_SecondaryAction() { return OnControllerInput_SecondaryAction; }
	FOnControllerInput_ToggleInventory& GetOnControllerInput_ToggleInventory() { return OnControllerInput_ToggleInventory; }
	FOnControllerInput_ToggleCharacterMenu& GetOnControllerInput_ToggleCharacterMenu() { return OnControllerInput_ToggleCharacterMenu; }
	FOnControllerInput_CancelAction& GetOnControllerInput_CancelAction() { return OnControllerInput_CancelAction; }
	FOnControllerInput_SelectActionBar& GetOnControllerInput_SelectActionBar() { return OnControllerInput_SelectActionBar; }

	void InitializeMinerController();*/
	

public:
	// Called every frame
	// virtual void Tick(float DeltaTime) override;
};
