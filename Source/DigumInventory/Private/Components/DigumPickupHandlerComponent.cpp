// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Components/DigumPickupHandlerComponent.h"

#include "Interface/DigumPickupInterface.h"
#include "Kismet/KismetSystemLibrary.h"

UDigumPickupHandlerComponent::UDigumPickupHandlerComponent()
{
}

void UDigumPickupHandlerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDigumPickupHandlerComponent::OnPickupEnabled(const bool bInEnabled)
{
	if (bInEnabled)
	{
		GetWorld()->GetTimerManager().SetTimer(PickupTimerHandle, this, &UDigumPickupHandlerComponent::UpdatePickupDetection, PickupDetectionFrequency, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(PickupTimerHandle);
	}
}

void UDigumPickupHandlerComponent::UpdatePickupDetection()
{
	if (!bPickupEnabled)
	{
		return;
	}

	TArray<FHitResult> OutHitResult;
	const FVector Start = GetOwner()->GetActorLocation();
	const FVector End =  GetOwner()->GetActorLocation();

	bool bHitResult;

	if(bDebug)
	{
		bHitResult =
			UKismetSystemLibrary::SphereTraceMulti(GetWorld(),
				Start,
				End,
				PickupRadius,
				UEngineTypes::ConvertToTraceType(ECC_Camera),
				false,
				TArray<AActor*>(),
				EDrawDebugTrace::ForDuration,
				OutHitResult, true,
				FColor::Green,
				FColor::Red,
				0.1f);
			}
	else
	{
		bHitResult = UKismetSystemLibrary::SphereTraceMulti(GetWorld(),
			Start,
			End,
			PickupRadius,
			UEngineTypes::ConvertToTraceType(ECC_Camera),
			false,
			TArray<AActor*>(),
			EDrawDebugTrace::None,
			OutHitResult, true);
	}
		
	if(bHitResult)
	{
		for(auto HitResult : OutHitResult)
		{
			if(AActor* HitActor = HitResult.GetActor())
			{
				// Check if the actor is a pickup
				if(HitActor->GetClass()->ImplementsInterface(UDigumPickupInterface::StaticClass()))
				{
					OnPickupDetected.Broadcast(HitActor);
					
				}
			}
		}
	}
}

void UDigumPickupHandlerComponent::SetPickupEnabled(const bool bInEnabled)
{
	bPickupEnabled = bInEnabled;

	OnPickupEnabled(bPickupEnabled);
}

void UDigumPickupHandlerComponent::SetPickupRadius(const float InRadius)
{
	PickupRadius = InRadius;
}

bool UDigumPickupHandlerComponent::IsPickupEnabled() const
{
	return bPickupEnabled;
}
