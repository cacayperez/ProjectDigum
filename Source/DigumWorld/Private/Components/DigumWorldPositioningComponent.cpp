// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Components/DigumWorldPositioningComponent.h"

#include "Functions/DigumWorldFunctionHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Settings/DigumWorldSettings.h"


// Sets default values for this component's properties
UDigumWorldPositioningComponent::UDigumWorldPositioningComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	

	SetComponentTickInterval(0.1f);
	// ...
}

void UDigumWorldPositioningComponent::CheckCoordinateChange()
{
	FDigumWorldProceduralSectionCoordinate NewCoordinate;
	FVector ActorLoc = GetOwner()->GetActorLocation();
	const FVector Location = FVector(ActorLoc.X, 0, ActorLoc.Z) - WorldOffset;
	UDigumWorldFunctionHelpers::ConvertToSectionCoordinates(Location , UnitSectionSize, NewCoordinate);
	NewCoordinate.X  = NewCoordinate.X <= 0 ? 0 : NewCoordinate.X - 1;
	NewCoordinate.Y  = NewCoordinate.Y <= 0 ? 0 : NewCoordinate.Y - 1;

	// UE_LOG(LogTemp , Warning , TEXT("New Coordinate: %s") , *NewCoordinate.ToString());	
	if(NewCoordinate.X != CurrentCoordinate.X || NewCoordinate.Y != CurrentCoordinate.Y)
	{
		PreviousCoordinate = CurrentCoordinate;
		CurrentCoordinate = NewCoordinate;
		
		//OnSectionCoordinateChanged.Broadcast(GetOwner(), CurrentCoordinate, PreviousCoordinate);
	}
	const FDigumWorldPositioningParams Params = FDigumWorldPositioningParams(PlayerId, ActorLoc);
	OnSectionCoordinateChanged.Broadcast(Params);
}


// Called when the game starts
void UDigumWorldPositioningComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner())
	{
		if(ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			if(const APlayerController* PlayerController = Character->GetController<APlayerController>())
			{
				if(const APlayerState* PlayerState = PlayerController->PlayerState)
				{
					PlayerId = PlayerState->GetUniqueId();
				}
			}
		}
		
	}
	/*if(GetWorld())
	{
		// TODO Get Current Content Category from Game Mode
		const FName CategoryName = TEXT("Primary");
		if(const UDigumWorldSettings* Settings = GetDefault<UDigumWorldSettings>())
		{
			const FDigumWorldContentCategory* Category = Settings->GetWorldContentCategoryData(CategoryName);
			if(Category)
			{
				/*GridSize = GetDefault<UDigumWorldSettings>()->GetGridSize();
				SectionWidth = Category->ProceduralRules.SectionWidth;
				SectionHeight = Category->ProceduralRules.SectionHeight;#1#
				
				const float UnitSectionWidth = SectionWidth * GridSize.X;
				const float UnitSectionHeight = SectionHeight * GridSize.Z;
				UnitSectionSize = FVector2D(UnitSectionWidth,  UnitSectionHeight);

				const float TotalWidth = Category->ProceduralRules.SectionCount_HorizontalAxis * UnitSectionWidth;
				const float TotalHeight = Category->ProceduralRules.SectionCount_VerticalAxis * UnitSectionHeight;

				const FVector Offset = (FVector(-TotalWidth / 2, 0, TotalHeight / 2));
				WorldOffset = Offset;
			}
		}
	}*/
}

void UDigumWorldPositioningComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bInitialized == true)
		CheckCoordinateChange();
	// UE_LOG(LogTemp, Warning, TEXT("Current Coordinate: %i, %i"), CurrentCoordinate.X, CurrentCoordinate.Y);
}

FDigumWorldProceduralSectionCoordinate UDigumWorldPositioningComponent::GetCurrentCoordinate() const
{
	return CurrentCoordinate;
}

void UDigumWorldPositioningComponent::InitializePositioningComponent(const FVector& InGridSize,
	const int32& InSectionWidth, const int32& InSectionHeight, const FVector& InWorldOffset)
{
	UE_LOG(LogTemp, Warning, TEXT("InitializePositioningComponent"));
	GridSize = InGridSize;
	SectionWidth = InSectionWidth;
	SectionHeight = InSectionHeight;
	UnitSectionSize = FVector2D(InSectionWidth * InGridSize.X, InSectionHeight * InGridSize.Z);
	WorldOffset = InWorldOffset;
	bInitialized = true;
}

