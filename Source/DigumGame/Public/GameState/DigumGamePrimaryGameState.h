// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameState/DigumGameState.h"
#include "DigumGamePrimaryGameState.generated.h"

struct FDigumWorldRequestParams;
class ADigumWorldMapActor;
/**
 * 
 */
UCLASS()
class DIGUMGAME_API ADigumGamePrimaryGameState : public AGameState
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
	TObjectPtr<ADigumWorldMapActor> MapActor;
public:
	DECLARE_MULTICAST_DELEGATE(FOnGameWorldLoaded);

	FOnGameWorldLoaded OnGameWorldLoaded;
	void TryInitializeWorld(TSoftClassPtr<ADigumWorldMapActor> InMapActorClass);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	void OnMapInitialized();
	void InitializeWorld_Internal(TSoftClassPtr<ADigumWorldMapActor> InMapActorClass);

public:
	FOnGameWorldLoaded& GetOnGameWorldLoadedDelegate() { return OnGameWorldLoaded; }

	void TryExecuteRequest(const FDigumWorldRequestParams& InParams);
	
};
