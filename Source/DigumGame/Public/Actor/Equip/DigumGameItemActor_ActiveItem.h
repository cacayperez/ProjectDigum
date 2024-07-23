// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DigumItemActor.h"
#include "Components/DigumActionComponent.h"
#include "GameFramework/Actor.h"
#include "DigumGameItemActor_ActiveItem.generated.h"

class UDigumAction;
class UCapsuleComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogDigumGameItemActor_ActiveItem, Log, All);

UENUM()
enum EDigumGameItem_ActionKey : uint8
{
	DigumGameActionKey_Default,
	DigumGameActionKey_Primary,
	DigumGameActionKey_Secondary,
};

UCLASS()
class DIGUMGAME_API ADigumGameItemActor_ActiveItem : public ADigumItemActor
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Digum Item", meta=(AllowPrivateAccess="true"))
	TMap<TEnumAsByte<EDigumGameItem_ActionKey>, TSoftClassPtr<UDigumAction>> Actions;

	
	
public:
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController = nullptr;
	
	// Sets default values for this actor's properties
	ADigumGameItemActor_ActiveItem(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void BeginPlay() override;
public:
	void TryActivateItem(AActor* InInstigator, const EDigumGameItem_ActionKey& ActionKey = EDigumGameItem_ActionKey::DigumGameActionKey_Default);
	virtual void OnActivateItem(AActor* InInstigator, const EDigumGameItem_ActionKey ActionKey = EDigumGameItem_ActionKey::DigumGameActionKey_Default);
	TSubclassOf<UDigumAction> GetActionClass(const EDigumGameItem_ActionKey ActionKey) const;
	virtual void OnActionBegin(AActor* InInstigator, const EDigumGameItem_ActionKey& ActionKey);
	virtual void OnActionFinished(AActor* InInstigator, const EDigumGameItem_ActionKey& ActionKey);

};
