// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumActorPool.generated.h"



DECLARE_DELEGATE_OneParam(FOnSpawnActorFromPool, AActor*)
DECLARE_DELEGATE_OneParam(FOnDespawnActorFromPool, AActor*)
/**
 * 
 */
UCLASS()
class DIGUMCORE_API UDigumActorPool : public UObject
{
	GENERATED_BODY()
public:
protected:
	UPROPERTY()
	UWorld* WorldContext;
	
	UPROPERTY()
	TArray<AActor*> InactiveActorPool;

	UPROPERTY()
	TArray<AActor*> ActiveActorPool;

	UPROPERTY()
	UClass* ClassType;
	
	FOnSpawnActorFromPool OnSpawnActorFromPoolDelegate;
	FOnDespawnActorFromPool OnDespawnActorFromPoolDelegate;
public:
	
	TArray<AActor*> GetInactiveActorPool() const { return InactiveActorPool; }
	TArray<AActor*> GetActiveActorPool() const { return ActiveActorPool; }
	
	void InitializePool(UWorld* InWorldContext, int32 InPoolSize, UClass* InClassType, const FName& InFolderPath = NAME_None);
	bool SpawnActorFromPool(const FVector& InLocation, const FRotator& InRotation);
	void DespawnActorFromPool(AActor* InActor);

	FOnSpawnActorFromPool& GetOnSpawnActorFromPoolDelegate() { return OnSpawnActorFromPoolDelegate; }
	FOnDespawnActorFromPool& GetOnDespawnActorFromPoolDelegate() { return OnDespawnActorFromPoolDelegate; }
};
