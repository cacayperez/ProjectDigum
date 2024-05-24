// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumItemActor.generated.h"

UCLASS()
class DIGUMINVENTORY_API ADigumItemActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Item", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Item", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Item", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector AttachPoint;
	
public:
	// Sets default values for this actor's properties
	ADigumItemActor(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }

	void SetAttachPoint(const FVector& InAttachPoint) { AttachPoint = InAttachPoint; }
};
