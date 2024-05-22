// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Components/DigumWorldISMComponent.h"


// Sets default values for this component's properties
UDigumWorldISMComponent::UDigumWorldISMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDigumWorldISMComponent::BeginPlay()
{
	Super::BeginPlay();

	// ... test
	for(int32 i = 0; i < 10; i++)
	{
		FTransform Transform;
		Transform.SetLocation(FVector(i * 100, 0, 0));
		AddInstance(Transform);
	}
}


// Called every frame
void UDigumWorldISMComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

