// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumAction.h"
#include "UObject/Object.h"
#include "DigumGameAnimatedAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DIGUMGAME_API UDigumGameAnimatedAction : public UDigumAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Digum Action", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AnimationMontage;

	UPROPERTY()
	UAnimInstance* AnimInstance;
protected:
	void OnMontageEnded(UAnimMontage* AnimMontage, bool bArg);
	virtual void OnExecuteAction(AActor* InExecutor) override;

public:
	virtual void InitializeDefaults() override;
};
