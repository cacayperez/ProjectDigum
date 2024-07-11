// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DigumGameCharacterSkinComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMGAME_API UDigumGameCharacterSkinComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDigumGameCharacterSkinComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
