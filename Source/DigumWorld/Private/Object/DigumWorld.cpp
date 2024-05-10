// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/DigumWorld.h"
#include "Object/DigumWorldLayer.h"

TArray<FDigumWorldLayer> UDigumWorld::GetLayers() const
{
	return Layers;
}
