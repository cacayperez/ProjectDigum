// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Components/DigumWorldMapLoaderComponent.h"

#include "Actor/DigumWorldDynamicProceduralActor.h"
#include "Async/DigumWorldAsyncSection.h"


// Sets default values for this component's properties
UDigumWorldMapLoaderComponent::UDigumWorldMapLoaderComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetIsReplicated(true);
}


// Called when the game starts
void UDigumWorldMapLoaderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if(GetOwner())
	{
		if(ADigumWorldDynamicProceduralActor* Actor = Cast<ADigumWorldDynamicProceduralActor>(GetOwner()))
		{
			ProceduralActor = Actor;
		}
		
	}
	
}

void UDigumWorldMapLoaderComponent::AsyncLoadSection()
{
	if(ProceduralActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Procedural Actor is null"));
	}
	
	TSharedPtr<FDigumWorldProceduralSection> Section;
	if(!SectionQueue.IsEmpty())
	{
		SectionQueue.Peek(Section);
		if(Section)
		{
			//FString ClientName = FString::Printf(TEXT("Client %i"), GetNetMode());
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Try Add Section, %i, %i, %s"), Section->GetX(), Section->GetY(), *ClientName));

			//UE_LOG(LogTemp, Warning, TEXT("Enqueuing Section, %i, %i"), Section->GetX(), Section->GetY());
			
			if(ProceduralActor)
			{
				if(ProceduralActor->TryAddSection(*Section.Get()))
				{
					SectionQueue.Dequeue(Section);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Cant add Section"));
				}
				// TO DO when task completes
			}
			else
			{
				if(ProceduralActor == nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("Procedural Actor is null"));
				}
				UE_LOG(LogTemp, Warning, TEXT("Failed to Add Section"));
			}
			Section.Reset();
		}

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
	ProceduralActor = GetProceduralActor();
	 
	if(ProceduralActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Procedural Actor is null"));
		return;
	}
	
	// UE_LOG(LogTemp, Warning, TEXT("Requesting Section %d, %d"), InX, InY);
	if(InX < 0 || InY < 0)
	{
		return;
	}

	const FDigumWorldMap* Map = ProceduralActor->GetMap();
	const FDigumWorldProceduralDefinition* Definition = ProceduralActor->GetProceduralDefinition();

	if(Map == nullptr || Definition == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Map or Definition is null"));
		return;
	}
	
	AsyncTask(ENamedThreads:: AnyThread, [this, Map, Definition, InX, InY]()
	{
		(new FAutoDeleteAsyncTask<FDigumWorldAsyncSection>(this, Map, Definition, InX, InY))->StartBackgroundTask();
	});
}

ADigumWorldDynamicProceduralActor* UDigumWorldMapLoaderComponent::GetProceduralActor()
{
	if(ProceduralActor == nullptr)
	{
		ProceduralActor = Cast<ADigumWorldDynamicProceduralActor>(GetOwner());
	}
	return ProceduralActor;
}

