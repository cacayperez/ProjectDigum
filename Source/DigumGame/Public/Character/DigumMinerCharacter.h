// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/DigumCharacter.h"
#include "DigumMinerCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDigumMinerCharacter, Log, All);

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class DIGUMGAME_API ADigumMinerCharacter : public ADigumCharacter
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Digum Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Digum Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> CameraComponent;
	

protected:
	void Move(const FInputActionValue& InputActionValue);
	
public:
	// Sets default values for this character's properties
	ADigumMinerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	

public:
	virtual void PrimaryAction();
	virtual void SecondaryAction();
public:
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }
};
