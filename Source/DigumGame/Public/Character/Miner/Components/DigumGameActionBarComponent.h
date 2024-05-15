// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DigumGameActionBarComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMGAME_API UDigumGameActionBarComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<int32> ActionKeys;

	UPROPERTY()
	int32 ActionKeysNum = 10;

	UPROPERTY()
	int32 ActiveActionIndex = -1;
public:
	// Sets default values for this component's properties
	UDigumGameActionBarComponent();
	
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivateItemAction, const int32&, ItemIndex);

	FOnActivateItemAction OnActivateItemAction;
private:
	void ActivateAction_Internal(const int32& ItemIndex);

public:
	void InitializeActionKeys(const TArray<int32> ItemIndices);
	void SetActionIndex(const int32& ActionIndex, const int32& ItemIndex);
	void SetActiveAction(const int32& ActionIndex);
	void TryActivateAction(const int32& ActionIndex);
	void ActivateDefaultAction();
	

	FOnActivateItemAction& OnActivateItemActionDelegate() { return OnActivateItemAction; }
};
