// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/Miner/DigumMinerCharacter.h"
#include "Player/DigumPlayerController.h"
#include "Subsystem/DigumWorldSubsystem.h"
#include "DigumMinerPlayerController.generated.h"

class ADigumGameBackgroundActor;
class ADigumWorldMapActor;
class ADigumMinerCharacter;
struct FInputActionValue;

UCLASS()
class DIGUMGAME_API ADigumMinerPlayerController : public ADigumPlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Miner Player Controller", meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<ACharacter> DefaultPlayerCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Miner Player Controller", meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<ADigumGameBackgroundActor> DefaultBackgroundClass;

	UPROPERTY()
	bool bCharacterHasBeenInitialized = false;

	UPROPERTY()
	TObjectPtr<ADigumWorldMapActor> WorldMapActor = nullptr;

public:
	// Sets default values for this actor's properties
	ADigumMinerPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void InitializeController(ADigumMinerCharacter* InCharacter);
	virtual void TrySpawnPlayerCharacter(const FVector& InWorldLocation = FVector::Zero());
	virtual void TrySpawnWorldMapActor();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnPlayerCharacter(const FVector& InWorldLocation);

	UFUNCTION(Server, Reliable)
	void Server_SpawnPlayerCharacter(const FVector& InWorldLocation);

	UFUNCTION(NetMulticast, Reliable)
	void  Multicast_SpawnWorldMapActor();

	UFUNCTION(Client, Reliable)
	void Client_SpawnWorldMapActor();
	
	UFUNCTION(Server, Reliable)
	void Server_SpawnWorldMapActor();

	UFUNCTION(Server, Reliable)
	void Server_TryAddBlock(const FName& InBlockID, const FVector& InWorldLocation);

	UFUNCTION(Server, Reliable)
	void Server_TryAddBlock_UsingParams(const FDigumWorldRequestParams& InParams);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TryAddBlock_UsingParams(const FDigumWorldRequestParams& InParams);

	UFUNCTION(Server, Reliable)
	void Server_TryRemoveBlock(const FVector& InWorldLocation, const float& InScaledDamage);
	
	virtual void TryRequest(const EDigumWorld_Request InRequest, const TArray<FVector> InArrayLocation, const float& InScaledDamage);

protected:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMinerCharacterSpawned, ACharacter*)

	FOnMinerCharacterSpawned OnMinerCharacterSpawned;

	virtual void AddBlock_UsingParams_Internal(const FDigumWorldRequestParams& InParams);
	virtual void BeginPlay() override;
	virtual void InitializeBackground();
	virtual void SpawnPlayerCharacter_Internal(const FVector& InWorldLocation);
	virtual void SpawnWorldMapActor_Internal();
	
public:
	FOnMinerCharacterSpawned& GetOnMinerCharacterSpawned() { return OnMinerCharacterSpawned; }
	
	bool IsCharacterInitialized() const { return bCharacterHasBeenInitialized; }
	
	ADigumWorldMapActor* GetWorldMapActor() const { return WorldMapActor; }
	// void TryAddBlock(const FName& InBlockID, const FVector& InWorldLocation);
	void TryAddBlock_UsingParams(const FDigumWorldRequestParams& InParams);
	void TryRemoveBlock(const TArray<FVector>& InArrayLocation, const float& InScaledDamage);
};
