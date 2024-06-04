// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DigumVisibilityComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMCORE_API UDigumVisibilityComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<APawn> PlayerPawn;

	UPROPERTY()
	float VisibilityRadius = 100.0f;
public:
	// Sets default values for this component's properties
	UDigumVisibilityComponent();

protected:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSetVisibility, bool);

	FOnSetVisibility OnSetVisibilityDelegate;
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FOnSetVisibility& GetOnSetVisibilityDelegate() { return OnSetVisibilityDelegate; }
};
