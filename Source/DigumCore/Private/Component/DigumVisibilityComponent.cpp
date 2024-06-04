// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Component/DigumVisibilityComponent.h"

#include "Interface/IDigumPlayerCharacterInterface.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UDigumVisibilityComponent::UDigumVisibilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	

	// ...
}


// Called when the game starts
void UDigumVisibilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(PlayerPawn.Get() && PlayerPawn->GetClass()->ImplementsInterface(UIDigumPlayerCharacterInterface::StaticClass()))
	{
		TScriptInterface<IIDigumPlayerCharacterInterface> Interface = PlayerPawn.Get();
		if(Interface)
		{
			VisibilityRadius = Interface->GetVisibilityRadius();
		}
	}
}


// Called every frame
void UDigumVisibilityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if(GetOwner() && PlayerPawn.Get())
	{
		const float Distance = FMath::Abs(FVector::Dist(GetOwner()->GetActorLocation(), PlayerPawn->GetActorLocation()));
		
		
		const bool bIsVisible = Distance >= VisibilityRadius;

		OnSetVisibilityDelegate.Broadcast(bIsVisible);
	}
}

