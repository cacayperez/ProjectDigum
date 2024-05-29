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

void UDigumWorldSubsystem::Request(const EDigumWorld_Request& InRequest, const FDigumWorldRequestParams& InParams) const
{
	OnWorldRequest.Broadcast(InRequest, InParams);
}
