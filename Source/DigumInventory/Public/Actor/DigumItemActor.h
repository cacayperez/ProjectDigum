// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Properties/DigumItemProperties.h"
#include "DigumItemActor.generated.h"


UCLASS()
class DIGUMINVENTORY_API ADigumItemActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Item", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Item", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	FDigumItemProperties ItemProperties;

public:
	// Sets default values for this actor's properties
	ADigumItemActor(const FObjectInitializer& ObjectInitializer);
	~ADigumItemActor();

protected:
	UPROPERTY()
	TObjectPtr<AActor> ItemInstigator;
	
public:
	
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }
	void SetItemInstigator(AActor* InInstigator);

	AActor* GetItemInstigator() const { return ItemInstigator; }
	void SetItemProperties(const FDigumItemProperties& InItemProperties);
	FDigumItemProperties* GetItemProperties();
};
