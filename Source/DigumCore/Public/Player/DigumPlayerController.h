// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DigumPlayerController.generated.h"

USTRUCT()
struct FDigumPlayerInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	FString PlayerID;

	UPROPERTY()
	int32 PlayerSessionID = -1;
};

UCLASS()
class DIGUMCORE_API ADigumPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	FDigumPlayerInfo PlayerInfo;
	
public:
	// Sets default values for this actor's properties
	ADigumPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FDigumPlayerInfo GetPlayerInfo() const { return PlayerInfo; }
	void SetPlayerInfo(const FDigumPlayerInfo& InPlayerInfo) { PlayerInfo = InPlayerInfo; }
	void SetPlayerSessionID(const int32 InPlayerSessionID) { PlayerInfo.PlayerSessionID = InPlayerSessionID; }
};
