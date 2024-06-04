// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DigumGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMCORE_API ADigumGameMode : public AGameMode
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Digum Game State", meta=(AllowPrivateAccess="true"))
	FName WorldContentCategoryName = TEXT("Primary");
	
public:
	ADigumGameMode();

	FName GetWorldContentCategory() const;
	void SetWorldContentCategory(const FName& InContentCategoryName);
};
