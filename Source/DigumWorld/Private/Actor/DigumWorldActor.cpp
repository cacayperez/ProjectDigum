// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldActor.h"

#include "Components/DigumWorldLayerComponent.h"
#include "Object/DigumWorld.h"
#include "Object/DigumWorldLayer.h"


// Sets default values
ADigumWorldActor::ADigumWorldActor()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

void ADigumWorldActor::BeginPlay()
{
	Super::BeginPlay();

	// Initialize layers
	if(WorldDefinitionClass)
	{
		WorldDefinition = NewObject<UDigumWorld>(this, WorldDefinitionClass);
	}
	
	if(WorldDefinition)
	{
		TArray<FDigumWorldLayer> LayerArray = WorldDefinition->GetLayers();
		for(int32 i = 0; i < LayerArray.Num(); i++)
		{
			const FDigumWorldLayer* Layer = &LayerArray[i];
			if(UDigumWorldLayerComponent* LayerComponent = NewObject<UDigumWorldLayerComponent>(this, UDigumWorldLayerComponent::StaticClass()))
			{
				
				LayerComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
				LayerComponent->SetStaticMesh(Layer->GetLayerMesh());
				LayerComponent->RegisterComponent();
				WorldLayers.Add(LayerComponent);
			}
		}
		
	}

}

void ADigumWorldActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
