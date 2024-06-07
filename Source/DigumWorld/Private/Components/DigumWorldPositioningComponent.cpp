// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Components/DigumWorldPositioningComponent.h"

#include "Functions/DigumWorldFunctionHelpers.h"
#include "Settings/DigumWorldSettings.h"


// Sets default values for this component's properties
UDigumWorldPositioningComponent::UDigumWorldPositioningComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// SetComponentTickInterval(0.2f);
	// ...
}

void UDigumWorldPositioningComponent::CheckCoordinateChange()
{
	
	FDigumWorldProceduralSectionCoordinate NewCoordinate;
	const FVector Location = GetOwner()->GetActorLocation() - WorldOffset;
	UDigumWorldFunctionHelpers::ConvertToSectionCoordinates(Location , UnitSectionSize, NewCoordinate);
	NewCoordinate.X  = NewCoordinate.X <= 0 ? 0 : NewCoordinate.X - 1;
	if(NewCoordinate != CurrentCoordinate)
	{
		PreviousCoordinate = CurrentCoordinate;
		CurrentCoordinate = NewCoordinate;
		OnSectionCoordinateChanged.Broadcast(GetOwner(), CurrentCoordinate, PreviousCoordinate);
		UE_LOG(LogTemp, Warning, TEXT("Current %s"), *CurrentCoordinate.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Previous %s"), *PreviousCoordinate.ToString());
	}
}


// Called when the game starts
void UDigumWorldPositioningComponent::BeginPlay()
{
	Super::BeginPlay();
	if(GetWorld())
	{
		// TODO Get Current Content Category from Game Mode
		const FName CategoryName = TEXT("Primary");
		if(const UDigumWorldSettings* Settings = GetDefault<UDigumWorldSettings>())
		{
			const FDigumWorldContentCategory* Category = Settings->GetWorldContentCategoryData(CategoryName);
			if(Category)
			{
				GridSize = GetDefault<UDigumWorldSettings>()->GetGridSize();
				SectionWidth = Category->ProceduralRules.SectionWidth;
				SectionHeight = Category->ProceduralRules.SectionHeight;
				
				const float UnitSectionWidth = SectionWidth * GridSize.X;
				const float UnitSectionHeight = SectionHeight * GridSize.Z;
				UnitSectionSize = FVector2D(UnitSectionWidth,  UnitSectionHeight);

				const float TotalWidth = Category->ProceduralRules.SectionCount_HorizontalAxis * UnitSectionWidth;
				const float TotalHeight = Category->ProceduralRules.SectionCount_VerticalAxis * UnitSectionHeight;

				const FVector Offset = (FVector(-TotalWidth / 2, 0, TotalHeight / 2));
				WorldOffset = Offset;
			}
		}
	}
}

void UDigumWorldPositioningComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckCoordinateChange();
	// UE_LOG(LogTemp, Warning, TEXT("Current Coordinate: %i, %i"), CurrentCoordinate.X, CurrentCoordinate.Y);
}

FDigumWorldProceduralSectionCoordinate UDigumWorldPositioningComponent::GetCurrentCoordinate() const
{
	return CurrentCoordinate;
}

