// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DigumGameCharacterSkinComponent.generated.h"

UENUM()
enum EDigumGame_BodySkinSlot : uint8
{
	DigumBodySkin_Head,
	DigumBodySkin_Body,
	DigumBodySkin_Tail,
	DigumBodySkin_BodyDetail_1,
	DigumBodySkin_FaceDetail_1,
	DigumBodySkin_BodyAccessory_1,
	DigumBodySkin_Miscellaneous_1
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMGAME_API UDigumGameCharacterSkinComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	TMap<EDigumGame_BodySkinSlot, FName> Character;
	
public:
	// Sets default values for this component's properties
	UDigumGameCharacterSkinComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void InitializeSkin(const TMap<EDigumGame_BodySkinSlot, const FName>& InSkinID);
	void SetSkeletalMesh(USkeletalMeshComponent* InSkeletalMesh);
	void TryModifySkin(const EDigumGame_BodySkinSlot& InSlot, const FName& InSkinID);
	
};
