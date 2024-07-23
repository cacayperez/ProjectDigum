// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Equip/DigumGameItemActor_ActiveItem.h"
#include "Subsystem/DigumWorldSubsystem.h"
#include "DigumGameItemActor_Block.generated.h"

struct FDigumWorldRequestParams;
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


	UPROPERTY(Replicated)
	FVector GridSize;

	UPROPERTY(Replicated)
	FVector TargetLocation;

	UPROPERTY(Replicated)
	FDigumWorldRequestParams RequestParams;
	
public:
	// Sets default values for this actor's properties
	ADigumGameItemActor_Block(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	bool TraceFromCursor(APlayerController* InPlayerController, FVector& OutLocation);

	UFUNCTION(Server, Reliable)
	void Server_SpawnBlockPreview();

	UFUNCTION(Client, Reliable)
	void Client_SpawnBlockPreview();
	
	UFUNCTION(Server, Reliable)
	void Server_ExecuteAction(const FDigumWorldRequestParams& InParams);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ExecuteAction(const FDigumWorldRequestParams& InParams);

	UFUNCTION(Client, Reliable)
	void Client_ExecuteAction(const FDigumWorldRequestParams& InParams);

	void ExecuteAction_Internal(const FDigumWorldRequestParams& InParams);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnActivateItem(AActor* InInstigator, const EDigumGameItem_ActionKey ActionKey) override;
};
