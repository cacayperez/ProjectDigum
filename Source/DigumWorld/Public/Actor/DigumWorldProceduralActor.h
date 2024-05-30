// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumWorldProceduralActor.generated.h"

class ADigumWorldActorSection;
struct FDigumWorldAssetCoordinateArray;
struct FDigumWorldProceduralCoordinateArray;

UCLASS()
class DIGUMWORLD_API ADigumWorldProceduralActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category = "Digum World Actor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY()
	TArray<TWeakObjectPtr<ADigumWorldActorSection>> SectionActors;

public:
	// Sets default values for this actor's properties
	ADigumWorldProceduralActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#if WITH_EDITOR
	UFUNCTION(BlueprintCallable, Category = "Digum World Actor", CallInEditor, meta = (DisplayName = "Generate World"))
	void Editor_GenerateProceduralWorld();

	UFUNCTION(BlueprintCallable, Category = "Digum World Actor", CallInEditor, meta = (DisplayName = "Clean World Asset"))
	void Editor_CleanActors();
#endif
};
