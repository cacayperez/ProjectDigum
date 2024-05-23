// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DigumActor.h"

ADigumActor::ADigumActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}
void ADigumActor::SetActorType(EDigumActorType InActorType)
{
	ActorType = InActorType;
}

