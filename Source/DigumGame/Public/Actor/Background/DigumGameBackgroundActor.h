// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumGameBackgroundActor.generated.h"

class ADigumMinerPlayerController;

UCLASS()
class DIGUMGAME_API ADigumGameBackgroundActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<USceneComponent> Root;

	UPROPERTY()
	TObjectPtr<ADigumMinerPlayerController> OwningPlayerController;
	
	UPROPERTY()
	TObjectPtr<ACharacter> TargetCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category="DigumGame | BackgroundSettings")
	float BackgroundDistanceOffset = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category="DigumGame | BackgroundSettings")
	float BackgroundZOffset = 0.0f;
	
public:
	// Sets default values for this actor's properties
	ADigumGameBackgroundActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void UpdateBackgroundPosition(const float& InDeltaTime);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Initialize(ADigumMinerPlayerController* MinerPlayerController);
	virtual void SetBackgroundEnabled(const bool& InValue);
	
};
