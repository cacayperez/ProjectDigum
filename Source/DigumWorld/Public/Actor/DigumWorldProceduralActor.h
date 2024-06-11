// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Procedural/DigumWorldMap.h"
#include "DigumWorldProceduralActor.generated.h"

struct FDigumWorldProceduralCoordinate;
struct FDigumWorldProceduralSectionCoordinate;
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
	/*void GenerateMap(const FName& InContentCategoryName);*/

protected:
	UPROPERTY(Replicated)
	FDigumWorldMap Map;

	UPROPERTY(Replicated)
	TArray<FDigumWorldProceduralSection> SectionDataArray;
	
	UPROPERTY()
	TObjectPtr<UDigumWorldProceduralAsset> ProceduralAsset;
	
	UPROPERTY()
	FVector2D UnitSectionSize;
	
	UPROPERTY()
	int32 LocalSectionWidth;

	UPROPERTY()
	int32 LocalSectionHeight;

	UPROPERTY()
	FVector GridSize;

	UPROPERTY()
	FVector WorldOffset;
	
	UPROPERTY()
	TArray<ADigumWorldActorSection*> SectionActors;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void CheckAndSetNeighbors(FDigumWorldProceduralSection* InSection, const int32& NumOfHierarchies, FDigumWorldProceduralSection* InLeftSection, FDigumWorldProceduralSection* InRightSection, FDigumWorldProceduralSection* InTopSection, FDigumWorldProceduralSection* InBottomSection, int32 InLocalSectionWidth, int32 InLocalSectionHeight);
	void MarkForFoliage(FDigumWorldProceduralCoordinate* InCoordinate);
	virtual void OnGenerateMap(const FName InSeed, const FVector InGridSize, const int32 InSectionWidth, const int32 InSectionHeight, const int32 InSectionCount_HorizontalAxis, const int32 InSectionCount_VerticalAxis, const int32 InNumberOfHierarchies, UDigumWorldProceduralAsset* InProceduralAsset);

	// FDigumWorldProceduralSection& GetSectionData(const int32& InX, const int32& InY);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void GenerateMap(const FName InSeed, const FVector InGridSize, const int32 InSectionWidth, const int32 InSectionHeight, const int32 InSectionCount_HorizontalAxis, const int32 InSectionCount_VerticalAxis, const int32 InNumberOfHierarchies, UDigumWorldProceduralAsset* InProceduralAsset);
		void SetProceduralAsset(UDigumWorldProceduralAsset* InProceduralAsset) { ProceduralAsset = InProceduralAsset; }
	// void Initialize(const int32& InLocalSectionWidth, const int32& InLocalSectionHeight, const FVector& InGridSize);
	bool GetSection(const int32& InSectionX, const int32& InSectionY, FDigumWorldProceduralSection& OutSection) const;

	virtual void CreateSection(const float& InSectionWidth, const float& InSectionHeight, const FVector& InWorldOffset, FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* InProceduralAsset);
	virtual void CreateSection(FDigumWorldProceduralSection InSection);
	virtual void AddBlock(const FName& InBlockID, const FVector& InBlockLocation);
	void ApplyWorldOffsetPosition();
	virtual ADigumWorldActorSection* GetSectionActor(const int32& InX, const int32& InY) const;

	const FDigumWorldMap* GetMap() const { return &Map;}
	FDigumWorldProceduralSectionCoordinate GetSectionCoordinate(const FVector& InWorldLocation) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
#if WITH_EDITOR
	UFUNCTION(BlueprintCallable, Category = "Digum World Actor", CallInEditor, meta = (DisplayName = "Generate World"))
	void Editor_GenerateProceduralWorld();

	UFUNCTION(BlueprintCallable, Category = "Digum World Actor", CallInEditor, meta = (DisplayName = "Clean World Asset"))
	void Editor_CleanActors();
#endif
};
