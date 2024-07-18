// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Subsystem/DigumWorldSubsystem.h"

UDigumWorldSubsystem* UDigumWorldSubsystem::Get(const UWorld* InWorld)
{
	if (InWorld)
	{
		UDigumWorldSubsystem* Subsystem = InWorld->GetSubsystem<UDigumWorldSubsystem>();
		if (Subsystem)
		{
			// Example: Call a function on your subsystem
			return Subsystem;
		}
	}

	return nullptr;
}

void UDigumWorldSubsystem::MakeRequest(UWorld* WorldContext, const FDigumWorldRequestParams& InParams)
{
	if(UDigumWorldSubsystem* WorldSubsystem = UDigumWorldSubsystem::Get(WorldContext))
	{
		WorldSubsystem->Request(InParams);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UDigumWorldSubsystem::MakeRequest - Unable to fullfil request"));
	}
}


void UDigumWorldSubsystem::Request(const FDigumWorldRequestParams& InParams) const
{
	UE_LOG(LogTemp, Warning, TEXT("UDigumWorldSubsystem::Request - Creating request"));
	OnWorldRequest.Broadcast(InParams);
}
