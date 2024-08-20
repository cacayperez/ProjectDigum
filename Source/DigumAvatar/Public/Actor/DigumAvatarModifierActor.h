// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumAvatarModifierActor.generated.h"

class IDigumAvatarInterface;

UCLASS()
class DIGUMAVATAR_API ADigumAvatarModifierActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	TScriptInterface<IDigumAvatarInterface> AvatarInterface = nullptr;
	
public:
	// Sets default values for this actor's properties
	ADigumAvatarModifierActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
