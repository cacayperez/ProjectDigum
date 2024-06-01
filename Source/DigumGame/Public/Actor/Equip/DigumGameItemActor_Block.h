// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Equip/DigumGameItemActor_ActiveItem.h"
#include "DigumGameItemActor_Block.generated.h"

class ADigumBuildPreviewActor;
class ADigumBlockPreview;
class APlayerController;

UCLASS()
class DIGUMGAME_API ADigumGameItemActor_Block : public ADigumGameItemActor_ActiveItem
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<AActor> PreviewClass;
	
	UPROPERTY()
	TObjectPtr<ADigumBuildPreviewActor> BlockPreview;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY()
	FVector GridSize; 
public:
	// Sets default values for this actor's properties
	ADigumGameItemActor_Block(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	bool TraceFromCursor(APlayerController* InPlayerController, FVector& OutLocation);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnActivateItem(AActor* InInstigator, const EDigumGameItem_ActionKey ActionKey) override;
};
