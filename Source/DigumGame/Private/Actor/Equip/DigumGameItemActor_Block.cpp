// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/Equip/DigumGameItemActor_Block.h"

#include "Actor/DigumWorldProceduralActor.h"
#include "Actor/Build/Preview/DigumBlockPreview.h"
#include "DigumBuild/Public/Actor/DigumBuildPreviewActor.h"
#include "Functions/DigumGameItemHelperFunctions.h"
#include "Interface/IDigumPlayerCharacterInterface.h"
#include "Item/DigumGameItemAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/DigumWorldSettings.h"


// Sets default values
ADigumGameItemActor_Block::ADigumGameItemActor_Block(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADigumGameItemActor_Block::BeginPlay()
{
	Super::BeginPlay();

	const FDigumItemProperties* Properties = GetItemProperties();

	GridSize = UDigumWorldSettings::GetGridSize();
	
	if(GetItemInstigator() && GetItemInstigator()->GetClass()->ImplementsInterface(UIDigumPlayerCharacterInterface::StaticClass()))
	{
		TScriptInterface<IIDigumPlayerCharacterInterface> PlayerCharacterInterface = GetItemInstigator();
		PlayerController = PlayerCharacterInterface->GetPlayerController();
	}
	
	if(Properties->IsValid() && PlayerController.Get())
	{
		FVector WorldLocation;
		FVector WorldDirection;
		UDigumGameItemHelperFunctions::GetCursorToWorldLocation(PlayerController.Get(), WorldLocation, WorldDirection);
	
		const FName ItemID = Properties->GetItemID();
		const FName ContentCategory = Properties->GetContentCategory();

		if(const UDigumGameItemAsset* ItemAsset = UDigumGameItemHelperFunctions::GetGameItemAsset(ItemID, ContentCategory))
		{
			PreviewClass = ItemAsset->GetBuildPreviewActorClass();
			if(PreviewClass)
			{
				BlockPreview = GetWorld()->SpawnActorDeferred<ADigumBuildPreviewActor>(PreviewClass, FTransform::Identity);
				
				if(BlockPreview)
				{
					FVector Location = WorldLocation + WorldDirection * 100.0f;
					Location.Y = 0.0f;
					BlockPreview->SetTargetLocation(Location);
					BlockPreview->FinishSpawning(FTransform::Identity);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("BlockPreview is null"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("PreviewClass is null"));
			}
		}
		
	}
}

void ADigumGameItemActor_Block::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(BlockPreview)
	{
		BlockPreview->Destroy();
	}
}

bool ADigumGameItemActor_Block::TraceFromCursor(APlayerController* InPlayerController, FVector& OutLocation)
{
	if(InPlayerController == nullptr)
	{
		return false;
	}
	
	FVector WorldLocation;
	FVector WorldDirection;
	UDigumGameItemHelperFunctions::GetCursorToWorldLocation(InPlayerController, WorldLocation, WorldDirection);

	OutLocation = WorldLocation + WorldDirection * 1200.0f;
	OutLocation.Y = 0;
	// bool bHitResult = Line
	return true;
}


// Called every frame
void ADigumGameItemActor_Block::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector TargetLocation;
	if(BlockPreview && TraceFromCursor(PlayerController.Get(), TargetLocation))
	{
		BlockPreview->SetTargetLocation(TargetLocation, GridSize);
	}
}

void ADigumGameItemActor_Block::OnActivateItem(AActor* InInstigator, const EDigumGameItem_ActionKey ActionKey)
{
	Super::OnActivateItem(InInstigator, ActionKey);

	if(BlockPreview)
	{
		FVector TargetLocation = BlockPreview->GetPreviewTargetLocation();
		AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), ADigumWorldProceduralActor::StaticClass());
		if(Actor)
		{
			if(ADigumWorldProceduralActor* ProceduralActor = Cast<ADigumWorldProceduralActor>(Actor))
			{
				FName BlockID = GetItemProperties()->GetItemID();
				ProceduralActor->AddBlock(BlockID, TargetLocation);
			}
			
		}
	}
}

