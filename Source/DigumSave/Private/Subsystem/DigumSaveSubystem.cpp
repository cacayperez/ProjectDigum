// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Subsystem/DigumSaveSubystem.h"


UDigumSaveSubystem::UDigumSaveSubystem()
{
}

UDigumSaveSubystem* UDigumSaveSubystem::Get(const UWorld* InWorld)
{
	if (InWorld)
	{
		if (UDigumSaveSubystem* Subsystem = InWorld->GetSubsystem<UDigumSaveSubystem>())
		{
			return Subsystem;
		}
	}

	return nullptr;
}
