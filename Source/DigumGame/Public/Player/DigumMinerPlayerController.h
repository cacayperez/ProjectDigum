// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/Miner/DigumMinerCharacter.h"
#include "Player/DigumPlayerController.h"
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

public:
	// Sets default values for this actor's properties
	ADigumMinerPlayerController();
	
	virtual void InitializeController(ADigumMinerCharacter* InCharacter);
	/*virtual void TrySpawnPlayerCharacter(const FVector& InWorldLocation = FVector::Zero());
	virtual void TrySpawnWorldMapActor();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnPlayerCharacter(const FVector& InWorldLocation);

	UFUNCTION(Server, Reliable)
	void Server_SpawnPlayerCharacter(const FVector& InWorldLocation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnWorldMapActor();
	
	UFUNCTION(Server, Reliable)
	void Server_SpawnWorldMapActor();*/

	
protected:

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMinerCharacterSpawned, ACharacter*)

	FOnMinerCharacterSpawned OnMinerCharacterSpawned;
	
	virtual void BeginPlay() override;
	virtual void InitializeBackground();
	/*virtual void SpawnPlayerCharacter_Internal(const FVector& InWorldLocation);
	virtual void SpawnWorldMapActor_Internal();*/

public:
	FOnMinerCharacterSpawned& GetOnMinerCharacterSpawned() { return OnMinerCharacterSpawned; }
	
	bool IsCharacterInitialized() const { return bCharacterHasBeenInitialized; }
};
