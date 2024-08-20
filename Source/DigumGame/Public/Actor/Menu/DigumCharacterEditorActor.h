// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumCharacterEditorActor.generated.h"

class ADigumMinerCharacter;

UCLASS()
class DIGUMGAME_API ADigumCharacterEditorActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ADigumMinerCharacter> MinerCharacter;
	
public:
	// Sets default values for this actor's properties
	ADigumCharacterEditorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
