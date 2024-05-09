// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/DigumInventoryComponent.h"
#include "DigumGameInventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMGAME_API UDigumGameInventoryComponent : public UDigumInventoryComponent
{
	GENERATED_BODY()

protected:
	virtual bool BuildItemProperties(const FDigumItemProperties& InItemProperties, UDigumItem*& OutBuiltItem) override;
	
};
