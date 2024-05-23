// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DigumWorldSwatchAsset.generated.h"

class ADigumWorldActorChild;
/**
 * 
 */
UCLASS()
class DIGUMWORLD_API UDigumWorldSwatchAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UMaterialInstance> EditorMaterial;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UMaterialInstance> WorldMaterial;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UStaticMesh> SwatchMesh;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<ADigumWorldActorChild> ChildActorClass;

	UMaterialInstance* GetEditorMaterial() const;
	TSubclassOf<ADigumWorldActorChild> GetChildActorClass() const;
	UMaterialInstance* GetWorldMaterial() const;
	UStaticMesh* GetSwatchMesh() const;

};
