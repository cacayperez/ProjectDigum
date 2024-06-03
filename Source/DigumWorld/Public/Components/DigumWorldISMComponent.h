// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "DigumWorldISMComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMWORLD_API UDigumWorldISMComponent : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDigumWorldISMComponent();
	
private:
	bool RemoveInstanceInternal_Custom(int32 InstanceIndex, bool InstanceAlreadyRemoved);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool RemoveInstance(int32 InstanceIndex) override;
	void SetTint(const int32& InstanceIndex, const int32& InHierarchyIndex);
};
