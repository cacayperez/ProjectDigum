// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/Equip/DigumGameItemActor_Block.h"

#include "Actor/DigumWorldMapActor.h"
#include "DigumBuild/Public/Actor/DigumBuildPreviewActor.h"
#include "Functions/DigumGameItemHelperFunctions.h"
#include "GameState/DigumGamePrimaryGameState.h"
#include "Interface/IDigumPlayerCharacterInterface.h"
#include "Item/DigumGameItemAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/DigumMinerPlayerController.h"
#include "Settings/DigumWorldSettings.h"
#include "Subsystem/DigumWorldSubsystem.h"


// Sets default values
ADigumGameItemActor_Block::ADigumGameItemActor_Block(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ADigumGameItemActor_Block::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADigumGameItemActor_Block, RequestParams);
	DOREPLIFETIME(ADigumGameItemActor_Block, TargetLocation);
}


// Called when the game starts or when spawned
void ADigumGameItemActor_Block::BeginPlay()
{
	Super::BeginPlay();

	const FDigumItemProperties* Properties = GetItemProperties();

	GridSize = UDigumWorldSettings::GetGridSize();

	// const FDigumItemProperties* Properties = GetItemProperties();
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if(Properties->IsValid() && PlayerController && PlayerController->IsLocalController())
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
					BlockPreview->SetOwner(GetOwner());
					FVector Location = WorldLocation + WorldDirection * 100.0f;
					Location.Y = 0.0f;
					BlockPreview->SetTargetLocation(Location);
					BlockPreview->FinishSpawning(FTransform::Identity);
					BlockPreview->GetOnSetTargetLocationDelegate().AddLambda([this](const FVector& InLocation)
					{
						TargetLocation = InLocation;
					});
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
	/*
	if(GetItemInstigator() && GetItemInstigator()->GetClass()->ImplementsInterface(UIDigumPlayerCharacterInterface::StaticClass()))
	{
		TScriptInterface<IIDigumPlayerCharacterInterface> PlayerCharacterInterface = GetItemInstigator();
		PlayerController = PlayerCharacterInterface->GetPlayerController();
	}*/

	
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

void ADigumGameItemActor_Block::Server_SpawnBlockPreview_Implementation()
{
	if(HasAuthority())
	{
		Client_SpawnBlockPreview();
	}
}

void ADigumGameItemActor_Block::Client_SpawnBlockPreview_Implementation()
{
	
}

void ADigumGameItemActor_Block::Server_ExecuteAction_Implementation(const FDigumWorldRequestParams& InParams)
{
	if(HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server_ExecuteAction_Implementation: Add Block"));
		Multicast_ExecuteAction(InParams);
		// Client_ExecuteAction(InParams);
	}
}

void ADigumGameItemActor_Block::Multicast_ExecuteAction_Implementation(const FDigumWorldRequestParams& InParams)
{
	if(PlayerController && PlayerController->IsLocalController())
	{
		ExecuteAction_Internal(InParams);
	}
}


void ADigumGameItemActor_Block::Client_ExecuteAction_Implementation(const FDigumWorldRequestParams& InParams)
{
	UE_LOG(LogTemp, Warning, TEXT("Client_ExecuteAction_Implementation: Add Block"));
	ExecuteAction_Internal(InParams);
}

void ADigumGameItemActor_Block::ExecuteAction_Internal(const FDigumWorldRequestParams& InParams)
{

	if(PlayerController)
	{
		if(ADigumMinerPlayerController* MPC = Cast<ADigumMinerPlayerController>(PlayerController.Get()))
		{
			MPC->TryAddBlock(InParams.BlockID, InParams.HitLocation);
		}
	}
	/*if(AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), ADigumWorldMapActor::StaticClass()))
	{
		if(ADigumWorldMapActor* WorldMapActor = Cast<ADigumWorldMapActor>(Actor))
		{
			WorldMapActor->TryExecuteAction(InParams);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor is not DigumWorldMapActor"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("World Map Actor Class is null"));
	}*/
	/*if(APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if(ADigumMinerPlayerController* MPC = Cast<ADigumMinerPlayerController>(PC))
		{
			if(ADigumWorldMapActor* WorldMapActor = MPC->GetWorldMapActor())
			{
				WorldMapActor->TryExecuteAction(InParams);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("WorldMapActor is null"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerController is not DigumMinerPlayerController"));
		
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is null"));
	}*/
}

// Called every frame
void ADigumGameItemActor_Block::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Location;
	if(BlockPreview && TraceFromCursor(PlayerController.Get(), Location))
	{
		BlockPreview->SetTargetLocation(Location, GridSize);
	}
}

void ADigumGameItemActor_Block::OnActivateItem(AActor* InInstigator, const EDigumGameItem_ActionKey ActionKey)
{
	Super::OnActivateItem(InInstigator, ActionKey);
	FName BlockID = GetItemProperties()->GetItemID();
	RequestParams = FDigumWorldRequestParams();
	RequestParams.Request = EDigumWorld_Request::DigumWorldRequest_Add;
	RequestParams.Instigator = GetItemInstigator();
	RequestParams.HitLocation = TargetLocation;
	RequestParams.BlockID = BlockID;
	UE_LOG(LogTemp, Warning, TEXT("OnActivateItem: Add Block, %s"), *TargetLocation.ToString());
	ExecuteAction_Internal(RequestParams);
	
}

