// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DigumWorldSubsystem.generated.h"

UENUM()
enum EDigumWorld_Request : uint8
{
	DigumWorldRequest_Default,
	DigumWorldRequest_Destroy,
	DigumWorldRequest_Add,
};

USTRUCT(BlueprintType)
struct FDigumWorldRequestParams
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TEnumAsByte<EDigumWorld_Request> Request;

	UPROPERTY()
	FName BlockID;

	UPROPERTY()
	int32 HitInstanceIndex;

	UPROPERTY()
	FVector HitLocation;
	
	UPROPERTY()
	AActor* Instigator;

	UPROPERTY()
	float Magnitude;

};
/**	
 * 
 */
UCLASS()
class DIGUMWORLD_API UDigumWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	

protected:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnWorldRequest, const FDigumWorldRequestParams&);
	FOnWorldRequest OnWorldRequest;

public:
	static UDigumWorldSubsystem* Get(const UWorld* InWorld);

	FOnWorldRequest& GetOnWorldRequestDelegate() { return OnWorldRequest; }
	void Request(const FDigumWorldRequestParams& InParams) const;	
	

	
	static void MakeRequest(UWorld* WorldContext, const FDigumWorldRequestParams& InParams);
};
