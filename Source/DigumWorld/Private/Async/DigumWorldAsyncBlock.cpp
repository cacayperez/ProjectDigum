// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldAsyncBlock.h"

#include "Actor/DigumWorldActorChild.h"

FDigumWorldAsyncBlock::FDigumWorldAsyncBlock(ADigumWorldActorChild* InActorChild, const FName& InBlockID,
	const FVector& InGridSize, const FVector& InPositionOffset,
	const FDigumWorldProceduralCoordinateArray& InCoordinates, const FDigumWorldRequestParams& InParams)
{
	ActorChild = InActorChild;
	BlockID = InBlockID;
	GridSize = InGridSize;
	PositionOffset = InPositionOffset;
	Coordinates = InCoordinates;
	Params = InParams;
}

FDigumWorldAsyncBlock::~FDigumWorldAsyncBlock()
{
	
}

void FDigumWorldAsyncBlock::DoWork()
{
	if(ActorChild == nullptr)
	{
		return;
	}

	const float GridX = GridSize.X;
	const float GridY = GridSize.Y;
	const float GridZ = GridSize.Z;

	int32 LoopCount = 0;
	TSharedPtr<FDigumWorldAsyncBlockResultArray> ResultArray = MakeShared<FDigumWorldAsyncBlockResultArray>();
	while(LoopCount < Coordinates.CoordinateCount())
	{
		const FDigumWorldProceduralCoordinate* Coordinate = Coordinates.GetCoordinate(LoopCount);
		
		// UE_LOG(LogTemp, Warning, TEXT("Hierarchy Index: %i"), Coordinate->Hierarchy);
		const int32 Variant = Coordinate->GetVariant(BlockID);
		const float X = Coordinate->X * GridX;
		const float Y = Coordinate->Hierarchy * GridY * (20.0f * -Coordinate->Hierarchy);
		const float Z = -(Coordinate->Y * GridZ);
		FVector Location = FVector(X, Y, Z);
		FTransform Transform = FTransform(FRotator::ZeroRotator, Location + PositionOffset, FVector(1.0f));

		FDigumWorldAsyncBlockResult Result;
		Result.BlockID = BlockID;
		Result.Coordinate = *Coordinate;
		Result.Transform = Transform;
		Result.Variant = Variant;
		Result.RequestParams = Params;
		ResultArray->Add(Result);
		LoopCount++;
	}

	ActorChild->AsyncBlockResultArrayQueue.Enqueue(ResultArray);
}
