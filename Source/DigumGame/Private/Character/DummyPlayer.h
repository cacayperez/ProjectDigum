// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DummyPlayer.generated.h"

class ADigumWorldDynamicProceduralActor;
class ADigumMinerCharacter;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class DIGUMGAME_API ADummyPlayer : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Dummy Player", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Dummy Player", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category= "Dummy Player", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> CameraComponent;
public:
	// Sets default values for this pawn's properties
	ADummyPlayer();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
