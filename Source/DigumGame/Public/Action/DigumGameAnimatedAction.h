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
public:
	~UDigumGameAnimatedAction();
protected:
	FOnMontageEnded MontageEndDelegate;
	void OnMontageEnded(UAnimMontage* AnimMontage, bool bArg);

	void OnBeginAnimationActionNotify();
	void OnEndAnimationActionNotify();	
	virtual void OnExecuteAction(AActor* InExecutor, UObject* InPayload = nullptr) override;

public:
	virtual void ExecuteAction(AActor* InExecutor, UObject* InPayload) override;
	virtual void EndAction(EDigumActionResult Result) override;
	virtual void InitializeDefaults() override;
	virtual bool IsBlockingAction() const override { return true; }
};
