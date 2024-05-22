// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumWorldActor.generated.h"

class ADigumWorldActorChild;
class UDigumWorldAsset;
class UDigumWorld;
class UDigumWorldISMComponent;

UCLASS()
class DIGUMWORLD_API ADigumWorldActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Digum World Actor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> WorldChildActors;
	
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum World Actor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDigumWorldAsset> WorldAsset;
#endif
	
protected:
	UPROPERTY()
	TObjectPtr<UDigumWorld> WorldDefinition;
public:
	// Sets default values for this actor's properties
	ADigumWorldActor();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#if WITH_EDITOR
	UFUNCTION(BlueprintCallable, Category = "Digum World Actor", CallInEditor, meta = (DisplayName = "Update World Asset"))
	void Editor_UpdateWorldAsset();

	UFUNCTION(BlueprintCallable, Category = "Digum World Actor", CallInEditor, meta = (DisplayName = "Clean World Asset"))
	void Editor_CleanActors();
#endif

};
