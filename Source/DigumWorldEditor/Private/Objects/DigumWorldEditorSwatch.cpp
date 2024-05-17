// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldEditorSwatch.h"
#include "Asset/DigumWorldSwatchAsset.h"

bool UDigumWorldEditorSwatch::IsValidSwatch() const
{
	return SoftSwatchAsset.IsValid();
}
