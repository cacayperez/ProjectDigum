// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/Equip/DigumGameItemActor_Block.h"
#include "DigumBuild/Public/Actor/DigumBuildPreviewActor.h"
#include "Functions/DigumGameItemHelperFunctions.h"
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
	bReplicates = true;

}

void ADigumGameItemActor_Block::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADigumGameItemActor_Block, RequestParams);
	DOREPLIFETIME(ADigumGameItemActor_Block, TargetLocation);
}

void ADigumGameItemActor_Block::SetTargetLocation(const FVector& InTargetLocation)
{
	Server_SetTargetLocation(InTargetLocation);
}

// Called when the game starts or when spawned
void ADigumGameItemActor_Block::BeginPlay()
{
	Super::BeginPlay();

	const FDigumItemProperties* Properties = GetItemProperties();

	GridSize = UDigumWorldSettings::GetGridSize();

	
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
					BlockPreview->GetOnSetTargetLocationDelegate().AddUObject(this, &ADigumGameItemActor_Block::SetTargetLocation);
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
		Multicast_ExecuteAction(InParams);
	}
}

void ADigumGameItemActor_Block::Multicast_ExecuteAction_Implementation(const FDigumWorldRequestParams& InParams)
{
	ENetMode NetMode = GetNetMode();

	if(NetMode == NM_ListenServer || NetMode == NM_DedicatedServer || NetMode == NM_Standalone)
	{
		ExecuteAction_Internal(InParams);
	}

	if(NetMode == NM_Client)
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
	
	if(PlayerController && PlayerController->IsLocalController())
	{
		if(ADigumMinerPlayerController* MPC = Cast<ADigumMinerPlayerController>(PlayerController.Get()))
		{
			MPC->TryAddBlock_UsingParams(InParams);
		}
	}
}

void ADigumGameItemActor_Block::Multicast_SetTargetLocation_Implementation(const FVector& InTargetLocation)
{
	TargetLocation = InTargetLocation;
}

void ADigumGameItemActor_Block::Client_SetTargetLocation_Implementation(const FVector& InTargetLocation)
{
	TargetLocation = InTargetLocation;
}

void ADigumGameItemActor_Block::Server_SetTargetLocation_Implementation(const FVector& InTargetLocation)
{
	if(HasAuthority())
	{
		Multicast_SetTargetLocation(InTargetLocation);
	}
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
	RequestParams.Amount = 1;
	RequestParams.SlotIndex = GetSlotIndex();
	int32 SessionID = -1;
	if(PlayerController)
	{
		SessionID = PlayerController->GetPlayerInfo().PlayerSessionID;
	}

	RequestParams.SessionID = SessionID;
	UE_LOG(LogTemp, Warning, TEXT("OnActivateItem: Add Block, %s, Session ID: %i"), *TargetLocation.ToString(), SessionID);

	Server_ExecuteAction(RequestParams);
	
}

