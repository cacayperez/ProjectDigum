// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DigumActor.h"
#include "GameFramework/Actor.h"
#include "Subsystem/DigumWorldSubsystem.h"
#include "DigumWorldActor.generated.h"

class ADigumWorldActorChild;
class UDigumWorldAsset;
class UDigumWorldISMComponent;

UCLASS()
class DIGUMWORLD_API ADigumWorldActor : public ADigumActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Digum World Actor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;

	UPROPERTY()
	TArray<TWeakObjectPtr<ADigumWorldActorChild>> WorldChildActors;


#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum World Actor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDigumWorldAsset> WorldAsset;
#endif
	
public:
	// Sets default values for this actor's properties
	ADigumWorldActor(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#if WITH_EDITOR
	UFUNCTION(BlueprintCallable, Category = "Digum World Actor", CallInEditor, meta = (DisplayName = "Update World Asset"))
	void Editor_UpdateWorldAsset();

	UFUNCTION(BlueprintCallable, Category = "Digum World Actor", CallInEditor, meta = (DisplayName = "Clean World Asset"))
	void Editor_CleanActors();
#endif

};
