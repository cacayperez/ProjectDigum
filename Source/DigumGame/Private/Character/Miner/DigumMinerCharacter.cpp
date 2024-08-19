// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/Miner/DigumMinerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actor/DigumWorldMapActor.h"
#include "Camera/CameraComponent.h"
#include "Character/Miner/Components/DigumGameActionBarComponent.h"
#include "Character/Miner/Components/DigumGameCharacterSkinComponent.h"
#include "Character/Miner/Components/DigumGameEquipComponent.h"
#include "Character/Miner/Components/DigumGameInventoryComponent.h"
#include "Components/DigumActionComponent.h"
#include "Components/DigumInventorySlot.h"
#include "Components/DigumPickupHandlerComponent.h"
#include "Components/DigumWorldPositioningComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/DigumInputSettingsAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/DigumPlayerController.h"
#include "Properties/DigumItem.h"
#include "Settings/DigumGameDeveloperSettings.h"

DEFINE_LOG_CATEGORY(LogDigumMinerCharacter);

constexpr int32 GDigum_ActionBarIndex_0 = 0;
constexpr int32 GDigum_ActionBarIndex_1 = 1;
constexpr int32 GDigum_ActionBarIndex_2 = 2;
constexpr int32 GDigum_ActionBarIndex_3 = 3;
constexpr int32 GDigum_ActionBarIndex_4 = 4;

void ADigumMinerCharacter::Server_TryActivateEquippedItemAction_Implementation(const FDigumGameActionParams& InActionParams)
{
	if(HasAuthority())
		Multicast_TryActivateEquippedItemAction(InActionParams);
}

void ADigumMinerCharacter::Multicast_TryActivateEquippedItemAction_Implementation(const FDigumGameActionParams& InActionParams)
{
	if(!IsLocallyControlled())
		ActivateEquippedItemAction_Internal(InActionParams);
}

void ADigumMinerCharacter::Server_SetFaceDirection_Implementation(float InDirection)
{
	if(HasAuthority())
		Multicast_SetFaceDirection(InDirection);
}

void ADigumMinerCharacter::Multicast_SetFaceDirection_Implementation(float InDirection)
{
	if(!IsLocallyControlled())
		SetFaceDirection(InDirection);
}

void ADigumMinerCharacter::Multicast_EquipItem_Implementation(const int32& InItemIndex)
{
	if(!IsLocallyControlled())
		EquipItem_Internal(InItemIndex);
}

void ADigumMinerCharacter::Server_EquipItem_Implementation(const int32& InItemIndex)
{
	if(HasAuthority())
		Multicast_EquipItem(InItemIndex);
}

void ADigumMinerCharacter::ActivateEquippedItemAction_Internal(const FDigumGameActionParams& InActionParams)
{
	if(GetEquipComponent())
	{
		ADigumItemActor* ItemActor = GetEquipComponent()->GetEquippedItemActor(InActionParams.EquipSlot);
		ADigumGameItemActor_ActiveItem* ActiveItemActor = Cast<ADigumGameItemActor_ActiveItem>(ItemActor);
		
		if(ActiveItemActor)
		{
			ActiveItemActor->TryActivateItem(this, InActionParams.ActionKey);
		}
		// GetEquipComponent()->ActivateAction(ActionParams);
	}
}

// Sets default values
ADigumMinerCharacter::ADigumMinerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SpringArmComponent->bDoCollisionTest = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Collision
	// GetCapsuleComponent()->SetCollisionObjectType(COLLISION_Miner);
	// GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_Miner, ECR_Ignore);
	
	// Custom Components
	InventoryComponent = CreateDefaultSubobject<UDigumGameInventoryComponent>(TEXT("InventoryComponent"));
	PickupHandlerComponent = CreateDefaultSubobject<UDigumPickupHandlerComponent>(TEXT("PickupHandlerComponent"));
	ActionBarComponent = CreateDefaultSubobject<UDigumGameActionBarComponent>(TEXT("ActionBarComponent"));
	ActionComponent = CreateDefaultSubobject<UDigumActionComponent>(TEXT("ActionComponent"));
	EquipComponent = CreateDefaultSubobject<UDigumGameEquipComponent>(TEXT("EquipComponent"));
	PositioningComponent = CreateDefaultSubobject<UDigumWorldPositioningComponent>(TEXT("PositioningComponent"));
	SkinComponent = CreateDefaultSubobject<UDigumGameCharacterSkinComponent>(TEXT("SkinComponent"));
}

void ADigumMinerCharacter::OnActivateEquippedItemAction(const FDigumGameActionParams& InActionParams)
{
	// Call function for prediction
	
	
	// Call Server function
	Server_TryActivateEquippedItemAction(InActionParams);
}

void ADigumMinerCharacter::OnActivateInventoryItemAction(const FDigumGameActionParams& InActionParams)
{
	
}

void ADigumMinerCharacter::EquipItem(const int32& InItemIndex)
{
	EquipItem_Internal(InItemIndex);
	
	Server_EquipItem(InItemIndex);
}

void ADigumMinerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogDigumMinerCharacter, Warning, TEXT("=== Character: Begin Play"))
	// Enable pickup detection
	// if(PickupHandlerComponent) PickupHandlerComponent->SetPickupEnabled(false);
	
	// constraint movement to the xz plane
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));
	
	GetCharacterMovement()->bConstrainToPlane = true;

	// Bind Action Bar Event
	if(GetActionBarComponent() && GetInventoryComponent() && GetEquipComponent())
	{
		GetInventoryComponent()->GetOnInventoryContentChangedDelegate().AddUObject(this, &ADigumMinerCharacter::OnInventoryContentChanged);
		TArray<UDigumInventorySlot*> Slots = GetInventoryComponent()->GetInventoryItems();
		TArray<int32> SlotIndices;
		for(int32 i = 0; i < 5; i++)
		{
			int32 SlotIndex = Slots[i]->GetInventoryIndex();
			SlotIndices.Add(SlotIndex);
		}
		GetActionBarComponent()->InitializeActionKeys(SlotIndices);
		GetActionBarComponent()->OnActivateItemActionDelegate().AddDynamic(this, &ADigumMinerCharacter::ActivateEquippedItemAction);
		EquipItem(GDigum_ActionBarIndex_0);
	}
	
	// Input and actions
	InitializeInputBindings();																																																																																																																																									
	
	// Pickup Handler

	if(PositioningComponent)
	{
		if(AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), ADigumWorldMapActor::StaticClass()))
		{
			if(ADigumWorldMapActor* WorldMapActor = Cast<ADigumWorldMapActor>(Actor))
			{
				UE_LOG(LogTemp, Warning, TEXT("Character Registering Positioning Component"));
				// WorldMapActor->RegisterPositioningComponent(PositioningComponent);
			}
			
		}
	}
}

void ADigumMinerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADigumMinerCharacter, FacedDirection);
}

void ADigumMinerCharacter::Move(const FInputActionValue& InputActionValue)
{
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	// UE_LOG(LogDigumMinerCharacter, Warning, TEXT("=== Character: Move"))
	if (Controller != nullptr)
	{
		// we only want to move on the x axis
		AddMovementInput(GetActorForwardVector(), MovementVector.X);

		// Predict
		SetFaceDirection(MovementVector.X);
		
		// Server Call
		Server_SetFaceDirection(MovementVector.X);
	}
}

void ADigumMinerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogDigumMinerCharacter, Warning, TEXT("=== Character: Setup input component"))
	// Super::SetupPlayerInputComponent(PlayerInputComponent);
	AssignedInputComponent = PlayerInputComponent;
	InitializeInputBindings(PlayerInputComponent);
	// 
	// InitializeInputBindings(PlayerInputComponent);
	// AssignedInputComponent = PlayerInputComponent;
}

void ADigumMinerCharacter::Jump()
{
	Super::Jump();
}

void ADigumMinerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADigumMinerCharacter::EquipItem_Internal(const int32& InItemIndex)
{
	if(GetInventoryComponent() && GetEquipComponent())
	{
		const TSubclassOf<ADigumItemActor> ItemActorClass = GetInventoryComponent()->GetItemActorClass(InItemIndex);
		FDigumItemProperties ItemProperties;
		GetInventoryComponent()->GetItemProperties(InItemIndex, ItemProperties);
		if(ItemActorClass) UE_LOG(LogDigumMinerCharacter, Warning, TEXT("ItemActorClass: %s"), *ItemActorClass->GetName());
		EquipComponent->EquipItem(ItemActorClass, ItemProperties, InItemIndex);

		if(GetActionComponent())
		{
			GetActionBarComponent()->SetActiveAction(InItemIndex);
		}
	}
}

void ADigumMinerCharacter::InitializeInputBindings(UInputComponent* InInputComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("=== Initializing input"));
	if(!GetController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is null"));
		return;
	}
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		const UDigumGameDeveloperSettings* DigumGameDeveloperSettings = GetDefault<UDigumGameDeveloperSettings>();
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (DigumGameDeveloperSettings && Subsystem)
		{
			const TSoftObjectPtr<UDigumInputSettingsAsset> InputSettingsAsset = DigumGameDeveloperSettings->PrimaryMinerInputSettings;
			if(const UDigumInputSettingsAsset* InputSettings = UDigumAssetManager::GetAsset<UDigumInputSettingsAsset>(InputSettingsAsset))
			{
				if(InputSettings->MappingContext)
					Subsystem->AddMappingContext(InputSettings->MappingContext, 0);
				else
				{
					UE_LOG(LogDigumMinerCharacter, Error, TEXT("PrimaryMinerInputSettings Mapping Context is null!"));
				}
			}
			else
			{
				UE_LOG(LogDigumMinerCharacter, Error, TEXT("Miner Input Settings is null!"));
			}
		}
		else
		{
			UE_LOG(LogDigumMinerCharacter, Error, TEXT("Failed to add mapping context"));
		}
	}
	else
	{
		UE_LOG(LogDigumMinerCharacter, Error, TEXT("Player Controller is null"));
	}
	
	if(AssignedInputComponent)
	{
		const UDigumGameDeveloperSettings* DigumGameDeveloperSettings = GetDefault<UDigumGameDeveloperSettings>();
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(AssignedInputComponent);
		if(DigumGameDeveloperSettings && EnhancedInputComponent)
		{
			if(const UDigumInputSettingsAsset* InputSettings = DigumGameDeveloperSettings->PrimaryMinerInputSettings.LoadSynchronous())
			{
				auto BindActionLambda = [&](const FString& Context, UInputAction* Action, ETriggerEvent TriggerEvent, auto Func)
				{
					if(Action != nullptr)
						EnhancedInputComponent->BindAction(Action, TriggerEvent, this, Func);
					else
					{
						UE_LOG(LogDigumMinerCharacter, Error, TEXT("%s Action is null!"), *Context);
					}
				};

				BindActionLambda("Move", InputSettings->MoveAction, ETriggerEvent::Triggered, &ADigumMinerCharacter::Move);
				BindActionLambda("Jump", InputSettings->JumpAction, ETriggerEvent::Started, &ADigumMinerCharacter::Jump);
				BindActionLambda("Primary",InputSettings->PrimaryAction, ETriggerEvent::Started, &ADigumMinerCharacter::PrimaryAction);
				BindActionLambda("Secondary",InputSettings->SecondaryAction, ETriggerEvent::Started,&ADigumMinerCharacter::SecondaryAction);
				BindActionLambda("Cancel",InputSettings->CancelAction, ETriggerEvent::Started,&ADigumMinerCharacter::CancelAction);
				BindActionLambda("Toggle Inventory", InputSettings->CharacterContextAction1, ETriggerEvent::Started, &ADigumMinerCharacter::ToggleInventory);
				BindActionLambda("Toggle Character Menu", InputSettings->CharacterContextAction2, ETriggerEvent::Started, &ADigumMinerCharacter::ToggleCharacterMenu);

				if(GetActionBarComponent())
				{
					BindActionLambda("ActionBar 0", InputSettings->ActionBar_Action_0, ETriggerEvent::Started, &ADigumMinerCharacter::SelectActionBar_0);
					BindActionLambda("ActionBar 1", InputSettings->ActionBar_Action_1, ETriggerEvent::Started, &ADigumMinerCharacter::SelectActionBar_1);
					BindActionLambda("ActionBar 2", InputSettings->ActionBar_Action_2, ETriggerEvent::Started, &ADigumMinerCharacter::SelectActionBar_2);
					BindActionLambda("ActionBar 3", InputSettings->ActionBar_Action_3, ETriggerEvent::Started, &ADigumMinerCharacter::SelectActionBar_3);
					BindActionLambda("ActionBar 4", InputSettings->ActionBar_Action_4, ETriggerEvent::Started, &ADigumMinerCharacter::SelectActionBar_4);
				}
			}
		}
		else
		{
			UE_LOG(LogDigumMinerCharacter, Error, TEXT("Failed to bind actions"));
		}
	}
	else
	{
		UE_LOG(LogDigumMinerCharacter, Error, TEXT("Input Component is null"));
	}
	
}

void ADigumMinerCharacter::ActivateEquippedItemAction(const FDigumGameActionParams& InActionParams)
{
	ActivateEquippedItemAction_Internal(InActionParams);

	Server_TryActivateEquippedItemAction(InActionParams);
}

FVector ADigumMinerCharacter::GetForwardDirection() const
{
	return GetActorForwardVector() * FacedDirection;
}

UDigumInventoryComponent* ADigumMinerCharacter::GetInventoryComponent() const
{
	return InventoryComponent;
}

UDigumWorldPositioningComponent* ADigumMinerCharacter::GetPositioningComponent() const
{
	return PositioningComponent;
}

UDigumActionComponent* ADigumMinerCharacter::GetActionComponent() const
{
	return ActionComponent;
}

UDigumActionComponent* ADigumMinerCharacter::GetActionComponentBP_Implementation() const
{
	return GetActionComponent();
}

void ADigumMinerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if(ADigumPlayerController* DigumPC = Cast<ADigumPlayerController>(NewController))
	{
		if(GetEquipComponent())
		{
			GetEquipComponent()->SetPlayerController(DigumPC);
		}
	}
}

void ADigumMinerCharacter::SetFaceDirection(float InDirection)
{
	FacedDirection = InDirection;
	UpdateMeshScale();
}

void ADigumMinerCharacter::PrimaryAction()
{
	if(GetActionBarComponent())
		GetActionBarComponent()->ActivatePrimaryAction();
}

void ADigumMinerCharacter::SecondaryAction()
{
}

void ADigumMinerCharacter::ToggleInventory()
{
	OnToggleInventory.Broadcast();
}

void ADigumMinerCharacter::ToggleCharacterMenu()
{
	OnToggleCharacterMenu.Broadcast();
}

void ADigumMinerCharacter::CancelAction()
{
	OnCancelAction.Broadcast();
}

void ADigumMinerCharacter::SelectActionBar(const int32& InActionIndex)
{
	EquipItem(InActionIndex);
}

void ADigumMinerCharacter::SelectActionBar_0()
{
	EquipItem(GDigum_ActionBarIndex_0);
}

void ADigumMinerCharacter::SelectActionBar_1()
{
	EquipItem(GDigum_ActionBarIndex_1);
}

void ADigumMinerCharacter::SelectActionBar_2()
{
	EquipItem(GDigum_ActionBarIndex_2);
}

void ADigumMinerCharacter::SelectActionBar_3()
{
	EquipItem(GDigum_ActionBarIndex_3);
}

void ADigumMinerCharacter::SelectActionBar_4()
{
	EquipItem(GDigum_ActionBarIndex_4);
}

void ADigumMinerCharacter::UpdateMeshScale()
{
	FVector Scale = GetMesh()->GetRelativeScale3D();
	if(FacedDirection < 0)
	{
		Scale.X = FMath::Abs(Scale.X) * -1;
		GetMesh()->SetRelativeScale3D(Scale);
		// GetArrowComponent()->SetRelativeScale3D(Scale);
	}
	else
	{
		Scale.X = FMath::Abs(Scale.X);
		GetMesh()->SetRelativeScale3D(Scale);
		// GetArrowComponent()->SetRelativeScale3D(Scale);
	}
}

void ADigumMinerCharacter::OnInventoryContentChanged()
{
	// Check for inventory and active action mismatch;
	if(GetActionBarComponent() && GetInventoryComponent() && GetEquipComponent())
	{
		const int32 ActiveActionIndex = GetActionBarComponent()->GetActiveActionIndex();
		ADigumItemActor* ItemActor = GetEquipComponent()->GetEquippedItemActor(EDigumGame_EquipSlot::DigumEquipSlot_MainHand);
		if(ActiveActionIndex != INDEX_NONE)
		{
			UE_LOG(LogDigumMinerCharacter, Warning, TEXT("ActiveActionIndex: %d"), ActiveActionIndex);
			const TSubclassOf<ADigumItemActor> ItemActorClass = GetInventoryComponent()->GetItemActorClass(ActiveActionIndex);

			// Check if item actor is already equipped
			if(ItemActorClass && !ItemActor)
			{
				EquipItem(ActiveActionIndex);
			}

			// Check if item actor has been removed
			if(!ItemActorClass && ItemActor)
			{
				GetEquipComponent()->ClearEquippedItem(EDigumGame_EquipSlot::DigumEquipSlot_MainHand);
			}

			// Check if item actor is different
			if(ItemActorClass && ItemActor && ItemActorClass != ItemActor->GetClass())
			{
				EquipItem(ActiveActionIndex);
			}
		}
	}	
}
