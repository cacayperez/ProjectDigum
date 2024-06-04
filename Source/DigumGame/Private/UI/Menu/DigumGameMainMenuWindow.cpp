// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/Menu/DigumGameMainMenuWindow.h"

#include "UI/Menu/SDigumGameMainMenuWindow.h"

void UDigumGameMainMenuWindow::OnCreateWidget()
{
	Super::OnCreateWidget();

	const float Width = ConstructParameters.Size.X;
	const float Height = ConstructParameters.Size.Y;

	MainMenuSlate =
	SNew(SDigumGameMainMenuWindow)
	.BackgroundMaterial(BackgroundMaterial)
	.WindowStyle(&WindowStyle)
	.HeightOverride(Height)
	.WidthOverride(Width) ;

}

TSharedPtr<SDigumWidget> UDigumGameMainMenuWindow::GetWidget() const
{
	return MainMenuSlate;
}
