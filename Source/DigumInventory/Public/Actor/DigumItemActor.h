// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Properties/DigumItemProperties.h"
#include "DigumItemActor.generated.h"

class ADigumPlayerController;

UCLASS()
class DIGUMINVENTORY_API ADigumItemActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Item", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Item", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(Replicated)
	FDigumItemProperties ItemProperties;

	UPROPERTY(Replicated)
	int32 SlotIndex = -1;

public:
	UPROPERTY(Replicated)
	TObjectPtr<ADigumPlayerController> PlayerController = nullptr;
	
	// Sets default values for this actor's properties
	ADigumItemActor(const FObjectInitializer& ObjectInitializer);
	~ADigumItemActor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	UPROPERTY()
	TObjectPtr<AActor> ItemInstigator;
	
public:
	
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }
	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }
	void SetItemInstigator(AActor* InInstigator);

	AActor* GetItemInstigator() const { return ItemInstigator; }
	void SetItemProperties(const FDigumItemProperties& InItemProperties);
	FDigumItemProperties* GetItemProperties();

	void SetPlayerController(ADigumPlayerController* InPlayerController);
	void SetSlotIndex(const int32 InSlotIndex);
};
