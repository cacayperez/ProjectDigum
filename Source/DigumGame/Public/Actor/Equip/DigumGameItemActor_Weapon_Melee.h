// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumGameItemActor_ActiveItem.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Subsystem/DigumWorldSubsystem.h"
#include "DigumGameItemActor_Weapon_Melee.generated.h"

class UArrowComponent;

UCLASS()
class DIGUMGAME_API ADigumGameItemActor_Weapon_Melee : public ADigumGameItemActor_ActiveItem
{
	GENERATED_BODY()

	UPROPERTY()
	FVector CollisionTraceLocation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Digum Item", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Digum Item | Weapon", meta=(AllowPrivateAccess="true"))
	FVector BoxTraceSize = FVector(30.0f, 10.0f, 20.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Digum Item | Weapon", meta=(AllowPrivateAccess="true"))
	float WeaponRange = 10000.0f;

	UPROPERTY()
	TArray<int32> HitItems;

public:

	// Sets default values for this actor's properties
	ADigumGameItemActor_Weapon_Melee(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	FTimerHandle CollisionTimerHandle;
	
public:
	float GetScaledDamage() const;
	virtual void OnTraceCollision();
	virtual void OnActionBegin(AActor* InInstigator, const EDigumGameItem_ActionKey& ActionKey) override;
	virtual void OnActionFinished(AActor* InInstigator, const EDigumGameItem_ActionKey& ActionKey) override;


	FORCEINLINE UBoxComponent* GetBoxComponent() const { return BoxComponent; }
};
