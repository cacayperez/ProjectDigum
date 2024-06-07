// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumWorldProceduralActor.generated.h"

struct FDigumWorldProceduralSection;
struct FDigumWorldAssetCoordinateArray;
struct FDigumWorldProceduralCoordinateArray;
class UDigumWorldProceduralAsset;
class ADigumWorldActorSection;

UCLASS()
class DIGUMWORLD_API ADigumWorldProceduralActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category = "Digum World Actor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;


	
public:
	// Sets default values for this actor's properties
	ADigumWorldProceduralActor();
	
private:
	void GenerateMap(const FName& InContentCategoryName);
protected:
	UPROPERTY()
	FVector2D SectionSize;
	
	UPROPERTY()
	int32 LocalSectionWidth;

	UPROPERTY()
	int32 LocalSectionHeight;

	UPROPERTY()
	FVector GridSize;
	
	UPROPERTY()
	TArray<ADigumWorldActorSection*> SectionActors;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Initialize(const int32& InLocalSectionWidth, const int32& InLocalSectionHeight, const FVector& InGridSize);
	virtual void CreateSection(const float& InSectionWidth, const float& InSectionHeight, const FVector& InWorldOffset, FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* ProceduralAsset);
	virtual void CreateSection(FDigumWorldProceduralSection& InSection, const FVector& InWorldOffset, UDigumWorldProceduralAsset* ProceduralAsset);
	virtual void AddBlock(const FName& InBlockID, const FVector& InBlockLocation);

	ADigumWorldActorSection* GetSectionActor(const int32& InX, const int32& InY) const;
#if WITH_EDITOR
	UFUNCTION(BlueprintCallable, Category = "Digum World Actor", CallInEditor, meta = (DisplayName = "Generate World"))
	void Editor_GenerateProceduralWorld();

	UFUNCTION(BlueprintCallable, Category = "Digum World Actor", CallInEditor, meta = (DisplayName = "Clean World Asset"))
	void Editor_CleanActors();
#endif
};
