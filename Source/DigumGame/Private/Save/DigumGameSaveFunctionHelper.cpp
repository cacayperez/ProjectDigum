// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Save/DigumGameSaveFunctionHelper.h"

#include "Kismet/GameplayStatics.h"
#include "Save/DigumGameSavedGameObject_Character.h"

void UDigumGameSaveFunctionHelper::CreateSaveCharacter(UDigumGameSavedGameObject_Character* InSavedGameObject,
                                                       const FString& InSaveSlotName, int32 InUserIndex)
{

	// FString 
	
	UGameplayStatics::SaveGameToSlot(InSavedGameObject, InSaveSlotName, InUserIndex);
	
}

FString UDigumGameSaveFunctionHelper::GetFullFolderPath(FString InLocalPath)
{
	// Ensure the folder exists or create it
	/*FString FullPath = InLocalPath;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*FolderPath))
	{
		PlatformFile.CreateDirectory(*FolderPath);

		if (!PlatformFile.DirectoryExists(*FolderPath))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create save game directory: %s"), *FolderPath);
			return;
		}
	}*/
	return FString();
}
