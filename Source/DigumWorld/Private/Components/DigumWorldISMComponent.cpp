// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Components/DigumWorldISMComponent.h"

#include "AI/NavigationSystemBase.h"

static TAutoConsoleVariable<int32> CVarISMForceRemoveAtSwap(
	TEXT("r.InstancedStaticMeshes.ForceRemoveAtSwap"),
	0,
	TEXT("Force the RemoveAtSwap optimization when removing instances from an ISM."));

// Sets default values for this component's properties
UDigumWorldISMComponent::UDigumWorldISMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// important to set this to true to avoid the performance hit of the old RemoveAt() behavior
	bSupportRemoveAtSwap = true;
	NumCustomDataFloats = 1;
	//SetCastShadow(true);
	bCastDynamicShadow = false;
}

bool UDigumWorldISMComponent::RemoveInstanceInternal_Custom(int32 InstanceIndex, bool InstanceAlreadyRemoved)
{
#if WITH_EDITOR
	DeletionState = InstanceAlreadyRemoved ? EInstanceDeletionReason::EntryAlreadyRemoved : EInstanceDeletionReason::EntryRemoval;
#endif

	// For performance we would prefer to use RemoveAtSwap() but some old code may be relying on the old
	// RemoveAt() behavior, since there was no explicit contract about how instance indices can move around.
	const bool bUseRemoveAtSwap = bSupportRemoveAtSwap || CVarISMForceRemoveAtSwap.GetValueOnGameThread() != 0;

	// remove instance
	if (!InstanceAlreadyRemoved && PerInstanceSMData.IsValidIndex(InstanceIndex))
	{
		const bool bWasNavRelevant = bNavigationRelevant;
		
		// Request navigation update
		PartialNavigationUpdate(InstanceIndex);

		if (bUseRemoveAtSwap)
		{
			PerInstanceSMData.RemoveAtSwap(InstanceIndex, 1, true);
			PerInstanceSMCustomData.RemoveAtSwap(InstanceIndex * NumCustomDataFloats, NumCustomDataFloats, false);
			
		}
		else
		{
			PerInstanceSMData.RemoveAt(InstanceIndex);
			PerInstanceSMCustomData.RemoveAt(InstanceIndex * NumCustomDataFloats, NumCustomDataFloats);
		}

		// If it's the last instance, unregister the component since component with no instances are not registered. 
		// (because of GetInstanceCount() > 0 in UInstancedStaticMeshComponent::IsNavigationRelevant())
		if (bWasNavRelevant && GetInstanceCount() == 0)
		{
			bNavigationRelevant = false;
			FNavigationSystem::UnregisterComponent(*this);
		}
	}

#if WITH_EDITOR
	// remove selection flag if array is filled in
	if (SelectedInstances.IsValidIndex(InstanceIndex))
	{
		if (bUseRemoveAtSwap)
		{
			SelectedInstances.RemoveAtSwap(InstanceIndex);
		}
		else
		{
			SelectedInstances.RemoveAt(InstanceIndex);
		}
	}
#endif

	const int32 LastInstanceIndex = PerInstanceSMData.Num();

	// update the physics state
	if (bPhysicsStateCreated && InstanceBodies.IsValidIndex(InstanceIndex))
	{
		if (FBodyInstance*& InstanceBody = InstanceBodies[InstanceIndex])
		{
			// Not having a body is a valid case when our physics state cannot be created (see CreateAllInstanceBodies)
			if(InstanceBody)
			{
				InstanceBody->TermBody();
				delete InstanceBody;
				InstanceBody = nullptr;
			}

			// --- Digum Fix for destroying collision bodies
			if(bUseRemoveAtSwap)
			{
				InstanceBodies.RemoveAtSwap(InstanceIndex, 1,true);
			}
			else
			{
				InstanceBodies.RemoveAt(InstanceIndex);
			}
			// --- end digum fix

			// Re-target instance indices for shifting of array.
			for (int32 i = InstanceIndex; i < InstanceBodies.Num(); ++i)
			{
				InstanceBodies[i]->InstanceBodyIndex = i;
			}
		}
	}

	// Notify that these instances have been removed/relocated
	if (FInstancedStaticMeshDelegates::OnInstanceIndexUpdated.IsBound())
	{
		if (bUseRemoveAtSwap)
		{
			TArray<FInstancedStaticMeshDelegates::FInstanceIndexUpdateData, TInlineAllocator<2>> IndexUpdates;
			IndexUpdates.Add(FInstancedStaticMeshDelegates::FInstanceIndexUpdateData{ FInstancedStaticMeshDelegates::EInstanceIndexUpdateType::Removed, InstanceIndex });
			if (InstanceIndex != LastInstanceIndex)
			{
				// used swap remove, so the last index has been moved to the spot we removed from
				IndexUpdates.Add(FInstancedStaticMeshDelegates::FInstanceIndexUpdateData{ FInstancedStaticMeshDelegates::EInstanceIndexUpdateType::Relocated, InstanceIndex, LastInstanceIndex });
			}

			FInstancedStaticMeshDelegates::OnInstanceIndexUpdated.Broadcast(this, IndexUpdates);
		}
		else
		{
			TArray<FInstancedStaticMeshDelegates::FInstanceIndexUpdateData> IndexUpdates;
			IndexUpdates.Reserve(IndexUpdates.Num() + 1 + (PerInstanceSMData.Num() - InstanceIndex));

			IndexUpdates.Add(FInstancedStaticMeshDelegates::FInstanceIndexUpdateData{ FInstancedStaticMeshDelegates::EInstanceIndexUpdateType::Removed, InstanceIndex });
			for (int32 MovedInstanceIndex = InstanceIndex; MovedInstanceIndex < PerInstanceSMData.Num(); ++MovedInstanceIndex)
			{
				// ISMs use standard remove, so each instance above our removal point is shuffled down by 1
				IndexUpdates.Add(FInstancedStaticMeshDelegates::FInstanceIndexUpdateData{ FInstancedStaticMeshDelegates::EInstanceIndexUpdateType::Relocated, MovedInstanceIndex, MovedInstanceIndex + 1 });
			}

			FInstancedStaticMeshDelegates::OnInstanceIndexUpdated.Broadcast(this, IndexUpdates);
		}
	}

	// Force recreation of the render data
	InstanceUpdateCmdBuffer.Edit();
	MarkRenderStateDirty();
#if WITH_EDITOR
	DeletionState = EInstanceDeletionReason::NotDeleting;
#endif
	return true;
}

// Called when the game starts
void UDigumWorldISMComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UDigumWorldISMComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UDigumWorldISMComponent::RemoveInstance(int32 InstanceIndex)
{
	return RemoveInstanceInternal_Custom(InstanceIndex, false);
}

void UDigumWorldISMComponent::SetTint(const int32& InstanceIndex, const int32& InHierarchyIndex)
{
	float Brightness = 1.0f;
	if(InstanceIndex != INDEX_NONE)
	{
		// Currently supporting 1 level of hierarchy
		// maybe consider multiple levels of hierarchy in the future
		// but for now, we will just use a single level of hierarchy, due to performance reasons
		if(InHierarchyIndex != 0)
		{
			Brightness = 0.2f;
		}
		else
		{
			Brightness = 1.0f;
		}
		SetCustomDataValue(InstanceIndex, 0, Brightness, true);
	}
}

