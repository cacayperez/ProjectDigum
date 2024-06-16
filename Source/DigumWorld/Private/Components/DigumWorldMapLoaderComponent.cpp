// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Components/DigumWorldMapLoaderComponent.h"

#include "Actor/DigumWorldDynamicProceduralActor.h"
#include "Async/DigumWorldAsyncSection.h"
#include "Procedural/DigumWorldMap.h"


// Sets default values for this component's properties
UDigumWorldMapLoaderComponent::UDigumWorldMapLoaderComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDigumWorldMapLoaderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UDigumWorldMapLoaderComponent::AsyncLoadSection()
{
	if(ProceduralActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Procedural Actor is null"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Async Load Section"));
	TSharedPtr<FDigumWorldProceduralSection> Section;
	if(!SectionQueue.IsEmpty() && SectionQueue.Dequeue(Section))
	{
		if(ProceduralActor && ProceduralActor->TryAddSection(*Section.Get()))
		{
			
			// TO DO when task completes
		}
		Section.Reset();
	}
}


// Called every frame
void UDigumWorldMapLoaderComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AsyncLoadSection();
}

void UDigumWorldMapLoaderComponent::InitializeDynamicProceduralMap(ADigumWorldDynamicProceduralActor* InProceduralActor)
{
	ProceduralActor = InProceduralActor;
}

void UDigumWorldMapLoaderComponent::RequestSection(const int32 InX, const int32 InY)
{
	if(ProceduralActor == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("Requesting Section %d, %d"), InX, InY);
	if(InX < 0 || InY < 0)
	{
		return;
	}

	const FDigumWorldMap* Map = ProceduralActor->GetMap();
	const FDigumWorldProceduralDefinition* Definition = ProceduralActor->GetProceduralDefinition();

	if(Map == nullptr || Definition == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Map or Definition is null"));
	}
	
	AsyncTask(ENamedThreads:: AnyThread, [this, Map, Definition, InX, InY]()
	{
		(new FAutoDeleteAsyncTask<FDigumWorldAsyncSection>(this, Map, Definition, InX, InY))->StartBackgroundTask();
	});
}

