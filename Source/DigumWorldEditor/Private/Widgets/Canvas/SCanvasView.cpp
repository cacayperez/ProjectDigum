// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SCanvasView.h"

#include "SlateMaterialBrush.h"
#include "SlateOptMacros.h"
#include "Rendering/DrawElements.h"
#include "Layout/Geometry.h"
#include "Asset/DigumWorldAsset.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Widgets/Base/SWidgetBase.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SCanvasView::~SCanvasView()
{
	OnSelectCanvasCoordinate.Clear();
	OnSetZoomFactor.Clear();
}

void SCanvasView::Construct(const FArguments& InArgs)
{
	Asset = InArgs._Asset;
	CanvasHeightAttribute = InArgs._CanvasHeight;
	CanvasWidthAttribute = InArgs._CanvasWidth;
	Zoom = InArgs._ZoomFactor.Get();
	_BackgroundGridPanel = SNew(SGridPanel);
		/*.Width(CanvasWidthAttribute.Get())
		.Height(CanvasHeightAttribute.Get())
		.SquareSize(SquareSize);*/

	_LayersPanel = SNew(SGridPanel);

	TestBrush = MakeShareable(new FSlateColorBrush(FLinearColor::Red));
	
	SWidgetBase::Construct(SWidgetBase::FArguments());
}

void SCanvasView::OnConstruct()
{
	_Container->ClearChildren();
	_BackgroundGridPanel->ClearChildren();
	_LayersPanel->ClearChildren();
	const int32 Width = CanvasWidthAttribute.Get();
	const int32 Height = CanvasHeightAttribute.Get();
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
		SNew(SBox)
		.RenderTransform(FSlateRenderTransform(Zoom))
		[
			_BackgroundGridPanel.ToSharedRef()
		]
	];

	_Container->AddSlot()
	[
		SNew(SBox)
		.RenderTransform(FSlateRenderTransform(Zoom))
		[
			
			_LayersPanel.ToSharedRef()
		]
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

FReply SCanvasView::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	float ScrollDelta = MouseEvent.GetWheelDelta();
	ProcessScroll(ScrollDelta);
	RefreshWidget();
	return SWidgetBase::OnMouseWheel(MyGeometry, MouseEvent);
}

FVector2D SCanvasView::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	const FVector2D Size = FVector2D(CanvasWidthAttribute.Get() * SquareSize * Zoom, CanvasHeightAttribute.Get() * SquareSize * Zoom);
	return Size;
}

void SCanvasView::ProcessScroll(const float InScrollDelta)
{
	Zoom = FMath::Clamp(Zoom + InScrollDelta * 0.1f, 0.1f, 2.0f);
	OnSetZoomFactor.Broadcast(Zoom);
}

int32 SCanvasView::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	int32 CurrentLayer = SWidgetBase::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
								bParentEnabled);

	
	return CurrentLayer;
}

void SCanvasView::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	ScaledSquareSize = SquareSize * Zoom;
	SWidgetBase::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
}

void SCanvasView::SelectCoordinate(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) const
{
	const FVector2D LocalPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	const int32 X = FMath::FloorToInt(LocalPosition.X / ScaledSquareSize);
	const int32 Y = FMath::FloorToInt(LocalPosition.Y / ScaledSquareSize);

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
