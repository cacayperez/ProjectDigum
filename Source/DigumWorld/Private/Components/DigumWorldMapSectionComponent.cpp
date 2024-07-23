// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Components/DigumWorldMapSectionComponent.h"

#include "Async/DigumWorldAsyncSectionLoader.h"


// Sets default values for this component's properties
UDigumWorldMapSectionComponent::UDigumWorldMapSectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	// SetIsReplicated(true);
	
}


// Called when the game starts
void UDigumWorldMapSectionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UDigumWorldMapSectionComponent::InitializeSections(const FDigumWorldProceduralRules& InProceduralRules, const FDigumWorldMap& InMap)
{
	
	OnInitializeSection.Broadcast();
	// Enable Tick
	SetAsyncCheck(true);

	// Initialize Sections Array
	TotalNumberOfSections = InProceduralRules.SectionCount_HorizontalAxis * InProceduralRules.SectionCount_VerticalAxis;
	SectionsArray.SetNum(TotalNumberOfSections);
	
	SectionWidth = InProceduralRules.SectionWidth;
	SectionHeight = InProceduralRules.SectionHeight;
	SectionCount_HorizontalAxis = InProceduralRules.SectionCount_HorizontalAxis;
	SectionCount_VerticalAxis = InProceduralRules.SectionCount_VerticalAxis;
	const int32 NumberOfHierarchies = InProceduralRules.NumberOfHierarchies;
	
	
	ProceduralAsset = InProceduralRules.GetProceduralAsset();
	WorldMap = InMap;
	
	if(ProceduralAsset)
	{
		for(int32 x = 0; x < SectionCount_HorizontalAxis; x++)
		{
			for(int32 y = 0; y < SectionCount_VerticalAxis; y++)
			{
				SectionsToGenerate.Add(TPair<int32, int32>(x, y));
				
		
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Procedural Asset is NULL"));
	}

	CheckSectionToGenerate();

}

void UDigumWorldMapSectionComponent::AsyncLoadSection()
{
	if(!SectionQueue.IsEmpty())
	{
		TSharedPtr<FDigumWorldProceduralSection> SectionPtr;
		if(SectionQueue.Dequeue(SectionPtr))
		{
			if(SectionPtr)
			{
				if(LoadSection(*SectionPtr.Get()))
				{
					SectionPtr.Reset();
					CheckSectionToGenerate();
				}
			}
		}
	}

	if(SectionQueue.IsEmpty() && SectionCounter == TotalNumberOfSections)
	{
		OnAllSectionsLoaded.Broadcast();
		SetAsyncCheck(false);
	}
}

bool UDigumWorldMapSectionComponent::LoadSection(FDigumWorldProceduralSection& Section)
{
	const int32 X = Section.GetX();
	const int32 Y = Section.GetY();
				
	// const int32 Index = (SectionCount_HorizontalAxis * X) + Y;
	const int32 Index = (SectionCount_HorizontalAxis * Y) + X;
	UE_LOG(LogTemp, Warning, TEXT("Section Loaded: %i, %i, %i"), X, Y, Index);
	if(Section.IsInitialized() && SectionsArray.IsValidIndex(Index))
	{

		SectionsArray[Index] = Section;
		OnSectionLoaded.Broadcast(Section);

		// Increment Section Counter
		SectionCounter++;
		return true;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Failed to Load Section: %i, %i"), X, Y);
	return false;
}

void UDigumWorldMapSectionComponent::SetAsyncCheck(const bool& bValue)
{
	SetComponentTickEnabled(bValue);
}

void UDigumWorldMapSectionComponent::CheckSectionToGenerate()
{
	
	TPair<int32, int32> SelectedPair;
	for(TPair<int32, int32> Pair : SectionsToGenerate)
	{
		
		SelectedPair = Pair;
		const int32 X = Pair.Key;
		const int32 Y = Pair.Value;
		if(X < 0 || Y < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid Section Coordinates"));
			return;
		}
		
		AsyncTask(ENamedThreads:: AnyThread, [this, X, Y]()
		{
			(new FAutoDeleteAsyncTask<FDigumWorldAsyncSectionLoader>(this, WorldMap, ProceduralAsset->ProceduralDefinition, X, Y))->StartBackgroundTask();
		});

		SectionsToGenerate.Remove(SelectedPair);
		break;
	}
}

FDigumWorldProceduralSection* UDigumWorldMapSectionComponent::GetSection(const int32& InSectionIndex)
{
	if(SectionsArray.IsValidIndex(InSectionIndex))
	{
		return &SectionsArray[InSectionIndex];
	}
	return nullptr;
}

void UDigumWorldMapSectionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AsyncLoadSection();
}




