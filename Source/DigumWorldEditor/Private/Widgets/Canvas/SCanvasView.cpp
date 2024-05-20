// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SCanvasView.h"

#include "SCanvasViewBackground.h"
#include "SlateMaterialBrush.h"
#include "SlateOptMacros.h"
#include "Asset/DigumWorldAsset.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Widgets/Base/SWidgetBase.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCanvasView::Construct(const FArguments& InArgs)
{
	Asset = InArgs._Asset;
	CanvasHeightAttribute = InArgs._CanvasHeight;
	CanvasWidthAttribute = InArgs._CanvasWidth;
	_BackgroundGridPanel = SNew(SCanvasViewBackground)
		.Width(CanvasWidthAttribute.Get())
		.Height(CanvasHeightAttribute.Get())
		.SquareSize(SquareSize);

	_LayersPanel = SNew(SGridPanel);
	
	SWidgetBase::Construct(SWidgetBase::FArguments());
}

void SCanvasView::OnConstruct()
{
	_Container->ClearChildren();
	_BackgroundGridPanel->ClearChildren();
	_LayersPanel->ClearChildren();
	int32 Width = CanvasWidthAttribute.Get();
	int32 Height = CanvasHeightAttribute.Get();
	for(int32 x = 0; x < Width; x++)
	{
		for(int32 y = 0; y < Height; y++)
		{
			FLinearColor Grey = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);
			FLinearColor White = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);
	
			FSlateColorBrush* Brush = new FSlateColorBrush((x + y) % 2 == 0 ? Grey : White);

			_BackgroundGridPanel->AddSlot(x, y)
			[
				SNew(SBox)
				.HeightOverride(SquareSize)
				.WidthOverride(SquareSize)
				[
					SNew(SImage)
					.Image(Brush)
				]
			];
		}
	}

	for(int32 i = 0; i < Asset.Get()->GetLayers().Num(); i++)
	{
		FDigumWorldAssetLayer* Layer = Asset.Get()->GetLayer(i);
		if(Layer->IsVisible() == false) continue;
		for(int32 x = 0; x < Width; x++)
		{
			for(int32 y = 0; y < Height; y++)
			{
				FDigumWorldAssetCoordinate* Coordinate = nullptr;
				if(Asset.Get()->GetCoordinate(i, x, y, Coordinate))
				{
					FDigumWorldSwatchPaletteItem* SwatchPaletteItem = Asset.Get()->GetSwatch(Coordinate->SwatchName);
					if(SwatchPaletteItem)
					{
						UDigumWorldSwatchAsset* SwatchAsset = SwatchPaletteItem->SoftSwatchAsset.LoadSynchronous();

						if(SwatchAsset)
						{
							UMaterialInstance* Material = SwatchAsset->EditorMaterial;
							if(Material)
							{
								FSlateMaterialBrush* MaterialBrush = new FSlateMaterialBrush(*Material, FVector2D(SquareSize, SquareSize));

								_LayersPanel->AddSlot(x,y)
								[
									SNew(SBox)
									.HeightOverride(SquareSize)
									.WidthOverride(SquareSize)
									[
										SNew(SImage)
										.Image(MaterialBrush)
									]
								];
						
							}
						}
					}
				}
				else
				{
					_LayersPanel->AddSlot(x, y)
						[
							SNew(SBox)
							.HeightOverride(SquareSize)
							.WidthOverride(SquareSize)
						];
				}
			}
		}
	}

	
	_Container->AddSlot()
	[
		_BackgroundGridPanel.ToSharedRef()
	];

	_Container->AddSlot()
	[
		_LayersPanel.ToSharedRef()
	];

}

FReply SCanvasView::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	StartDrag();
	SelectCoordinate(MyGeometry, MouseEvent);
	return SWidgetBase::OnMouseButtonDown(MyGeometry, MouseEvent).CaptureMouse(SharedThis(this));
}

FReply SCanvasView::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if(bIsDragging == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("DRAGG"));
		SelectCoordinate(MyGeometry, MouseEvent);
	}
	return SWidgetBase::OnMouseMove(MyGeometry, MouseEvent);
}

FReply SCanvasView::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	StopDrag();
	return SWidgetBase::OnMouseButtonUp(MyGeometry, MouseEvent).ReleaseMouseCapture();
}

FVector2D SCanvasView::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	const FVector2D Size = FVector2D(CanvasWidthAttribute.Get() * SquareSize, CanvasHeightAttribute.Get() * SquareSize);
	return Size;
}

void SCanvasView::SelectCoordinate(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) const
{
	const FVector2D LocalPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	const int32 X = FMath::FloorToInt(LocalPosition.X / SquareSize);
	const int32 Y = FMath::FloorToInt(LocalPosition.Y / SquareSize);

	OnSelectCanvasCoordinate.Broadcast(X, Y);
}

void SCanvasView::StartDrag()
{
	bIsDragging = true;
}

void SCanvasView::StopDrag()
{
	bIsDragging = false;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
