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

	UPROPERTY()
	int32 SessionID;

	UPROPERTY()
	int32 Amount;

	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	FString UniqueID;
	
	FString ToString() const
	{
		return FString::Printf(TEXT("Request: %s, BlockID: %s, HitInstanceIndex: %d, HitLocation: %s, Instigator: %s, Magnitude: %f,\\n SessionID: %i , Amount: %i, SlotIndex: %i"),
			*UEnum::GetValueAsString(Request.GetValue()), *BlockID.ToString(), HitInstanceIndex, *HitLocation.ToString(), *Instigator->GetName(), Magnitude, SessionID, Amount, SlotIndex);
	}
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
