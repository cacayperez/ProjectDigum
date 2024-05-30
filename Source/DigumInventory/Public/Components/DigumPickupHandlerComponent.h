// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumPickupHandlerComponent.generated.h"


/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMINVENTORY_API UDigumPickupHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Pickup Handler", meta=(AllowPrivateAccess="true"))
	bool bDebug = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Pickup Handler", meta=(AllowPrivateAccess="true"))
	float PickupRadius = 75.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Pickup Handler", meta=(AllowPrivateAccess="true"))
	bool bPickupEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Digum Pickup Handler", meta=(AllowPrivateAccess="true"))
	float PickupDetectionFrequency = 0.1f;
	
public:
	UDigumPickupHandlerComponent();

protected:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPickupDetected, AActor*);
	
	FOnPickupDetected OnPickupDetected;
	FTimerHandle PickupTimerHandle;

	virtual void BeginPlay() override;

	void OnPickupEnabled(const bool bInEnabled);
	void UpdatePickupDetection();
	
public:
	void SetPickupEnabled(const bool bInEnabled);
	void SetPickupRadius(const float InRadius);
	bool IsPickupEnabled() const;

	FOnPickupDetected& GetOnPickupDetectedDelegate() { return OnPickupDetected; }
};
