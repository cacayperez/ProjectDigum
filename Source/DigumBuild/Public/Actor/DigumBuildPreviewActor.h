// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumBuildPreviewActor.generated.h"

UCLASS()
class DIGUMBUILD_API ADigumBuildPreviewActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	FVector GridSize;
public:
	// Sets default values for this actor's properties
	ADigumBuildPreviewActor();

	virtual void Tick(float DeltaSeconds) override;
	void SetTargetLocation(const FVector& InTargetLocation, const FVector& InGridSize = FVector(100.0f, 100.0f, 100.0f));
	bool IsBlockOccupied(const FVector& InLocation, const FVector& InGridSize);
	FVector SnapToGrid(const FVector& InLocation, const FVector& InGridSize) const;
	FVector GetPreviewTargetLocation() const;
};
