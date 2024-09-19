// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Component/DigumAvatarComponent.h"

#include "GameFramework/Character.h"


// Sets default values for this component's properties
UDigumAvatarComponent::UDigumAvatarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDigumAvatarComponent::BeginPlay()
{
	Super::BeginPlay();

	// initialize mesh component
	SkeletalMeshComponent = GetMeshComponent();
}

USkeletalMeshComponent* UDigumAvatarComponent::GetMeshComponent()
{
	if(SkeletalMeshComponent == nullptr)
	{
		if(GetOwner())
		{
			if(ACharacter* Character = Cast<ACharacter>(GetOwner()))
			{
				SkeletalMeshComponent = Character->GetMesh();
			}
		}
		
	}
#if WITH_EDITOR
	if(SkeletalMeshComponent == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("UDigumAvatarComponent::GetMeshComponent, Skeletal mesh is null ptr"))
#endif
	

	return SkeletalMeshComponent;
}


// Called every frame
void UDigumAvatarComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

