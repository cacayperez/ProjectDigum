// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Object/DigumActorPool.h"

void UDigumActorPool::InitializePool(UWorld* InWorldContext, int32 InPoolSize, UClass* InClassType, const FName& InFolderPath )
{
	ClassType = InClassType;
	WorldContext = InWorldContext;
	for(int32 i = 0; i < InPoolSize; i++)
	{
		AActor* Actor = WorldContext->SpawnActor<AActor>(ClassType);
		Actor->SetFolderPath(InFolderPath);
		Actor->SetActorHiddenInGame(true);
		Actor->SetActorEnableCollision(false);
		InactiveActorPool.Add(Actor);
	}
}

bool UDigumActorPool::SpawnActorFromPool(const FVector& InLocation, const FRotator& InRotation)
{
	AActor* Actor = nullptr;

	if(InactiveActorPool.Num() > 0)
	{
		Actor = InactiveActorPool.Pop();
	}
	else if(ActiveActorPool.Num() > 0)
	{
		Actor = ActiveActorPool[0];
		ActiveActorPool.RemoveAt(0);
	}

	if(Actor)
	{
		Actor->SetActorLocation(InLocation);
		Actor->SetActorRotation(InRotation);
		Actor->SetActorHiddenInGame(false);
		Actor->SetActorEnableCollision(true);
		ActiveActorPool.Add(Actor);
		OnSpawnActorFromPoolDelegate.ExecuteIfBound(Actor);

		return true;
	}

	return false;
}

void UDigumActorPool::DespawnActorFromPool(AActor* InActor)
{
	if (InActor)
	{
		// InActor->OnDespawned();
		InActor->SetActorHiddenInGame(true);
		InActor->SetActorEnableCollision(false);

		OnDespawnActorFromPoolDelegate.ExecuteIfBound(InActor);
		
		ActiveActorPool.Remove(InActor);
		InactiveActorPool.Add(InActor);
	}
}

