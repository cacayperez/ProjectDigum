// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Procedural/DigumWorldMap.h"
// #include "Procedural/DigumWorldGenerator.h"
#include "DigumWorldMapSectionComponent.generated.h"


struct FDigumWorldProceduralSection;

// Class for managing sections
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMWORLD_API UDigumWorldMapSectionComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FDigumWorldProceduralSection> SectionsArray;

	UPROPERTY()
	int32 SectionCounter = 0;

	UPROPERTY()
	int32 TotalNumberOfSections = -1;

	TArray<TPair<int32, int32>> SectionsToGenerate;

	UPROPERTY()
	FDigumWorldMap WorldMap;

	UPROPERTY()
	UDigumWorldProceduralAsset* ProceduralAsset;
	
public:
	// Sets default values for this component's properties
	UDigumWorldMapSectionComponent();

protected:
	DECLARE_MULTICAST_DELEGATE(FOnInitializeSection);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSectionLoaded, FDigumWorldProceduralSection& Section);
	DECLARE_MULTICAST_DELEGATE(FOnAllSectionsLoaded);
	
	// Called when the game starts
	virtual void BeginPlay() override;
	

	void AsyncLoadSection();
	bool LoadSection(FDigumWorldProceduralSection& Section);

	FOnInitializeSection OnInitializeSection;
	FOnSectionLoaded OnSectionLoaded;
	FOnAllSectionsLoaded OnAllSectionsLoaded;
	void SetAsyncCheck(const bool& bValue);
	void CheckSectionToGenerate();
public:
	TQueue<TSharedPtr<FDigumWorldProceduralSection>> SectionQueue;
	
	FDigumWorldProceduralSection* GetSection(const int32& InSectionIndex);
	
	TArray<FDigumWorldProceduralSection>& GetSectionsArray()  { return SectionsArray; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnInitializeSection& GetOnInitializeSectionDelegate() { return OnInitializeSection; }
	FOnSectionLoaded& GetOnSectionLoadedEventDelegate() { return OnSectionLoaded; }
	FOnAllSectionsLoaded& GetOnAllSectionsLoadedEventDelegate() { return OnAllSectionsLoaded; }

	void InitializeSections(const FDigumWorldProceduralRules& InProceduralRules, const FDigumWorldMap& InMap);
};
