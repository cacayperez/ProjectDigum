// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumWorldActor.generated.h"

class UDigumWorld;
class UDigumWorldLayerComponent;

UCLASS()
class DIGUMWORLD_API ADigumWorldActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Digum World Actor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(BlueprintReadWrite, Category = "Digum World Actor", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UDigumWorldLayerComponent>> WorldLayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Digum World Actor", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDigumWorld> WorldDefinitionClass;

protected:
	UPROPERTY()
	TObjectPtr<UDigumWorld> WorldDefinition;
public:
	// Sets default values for this actor's properties
	ADigumWorldActor();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
