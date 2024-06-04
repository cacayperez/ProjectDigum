// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Components/DigumWorldPositioningComponent.h"

#include "GameMode/DigumGameMode.h"
#include "GameState/DigumGameState.h"
#include "Interface/IDigumPlayerCharacterInterface.h"


// Sets default values for this component's properties
UDigumWorldPositioningComponent::UDigumWorldPositioningComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDigumWorldPositioningComponent::BeginPlay()
{
	Super::BeginPlay();
	if(GetWorld())
	{
		ADigumGameState* GameState = GetWorld()->GetGameState<ADigumGameState>();

		// TODO: Implement this
		// Get the game settings.
	}
	
	/*
	if(GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UIDigumPlayerCharacterInterface::StaticClass()))
	{
		TScriptInterface<IIDigumPlayerCharacterInterface> Interface = GetOwner();
		if(Interface)
		{
			if(APlayerController* PlayerController = Interface->GetPlayerController())
			{
				
			}

			
		}
	}
	*/
	
	/*
	if(const UDigumWorldSettings* Settings = GetDefault<UDigumWorldSettings>())
	{
		const FDigumWorldContentCategory* Category = Settings->GetWorldContentCategoryData(InContentCategoryName);
		if(Category)
		{
			const FVector GridSize = GetDefault<UDigumWorldSettings>()->GetGridSize();
			const int32 SectionWidth = Category->ProceduralRules.SectionWidth;
			const int32 SectionHeight = Category->ProceduralRules.SectionHeight;
			const float UnitSectionWidth = SectionWidth * GridSize.X;
			const float UnitSectionHeight = SectionHeight * GridSize.Z;

			OutXCoordinate = FMath::FloorToInt(InWorldLocation.X / UnitSectionWidth);
			OutYCoordinate = -FMath::CeilToInt((InWorldLocation.Z) / UnitSectionHeight);

			return true;
		}
	}
	return false;
	 */
}


// Called every frame
void UDigumWorldPositioningComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

