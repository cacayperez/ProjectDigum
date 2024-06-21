// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DigumWorldMapLoaderComponent.generated.h"


struct FDigumWorldProceduralSection;
class ADigumWorldDynamicProceduralActor;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMWORLD_API UDigumWorldMapLoaderComponent : public UActorComponent
{
	GENERATED_BODY()

	
public:
	// Sets default values for this component's properties
	UDigumWorldMapLoaderComponent();

protected:

	UPROPERTY()
	TObjectPtr<ADigumWorldDynamicProceduralActor> ProceduralActor;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	void AsyncLoadSection();
public:
	
	TQueue<TSharedPtr<FDigumWorldProceduralSection>> SectionQueue;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeDynamicProceduralMap(ADigumWorldDynamicProceduralActor* InProceduralActor);
	void RequestSection(const int32 InX, const int32 InY);
	ADigumWorldDynamicProceduralActor* GetProceduralActor();

	// FORCEINLINE ADigumWorldDynamicProceduralActor* GetProceduralActor() const { return ProceduralActor; }
};
