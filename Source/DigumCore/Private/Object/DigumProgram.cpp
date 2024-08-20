// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Object/DigumProgram.h"

FString UDigumProgram::GenerateUniqueID()
{
	FString ProgramVersion = GetProgramVersion();
	FDateTime Now = FDateTime::UtcNow();

	FString DateTimeString = Now.ToString(TEXT("%Y%m%d%H%M%S"));
	FString UniqueID = FString::Printf(TEXT("%s-%s"), *ProgramVersion, *DateTimeString);

	FGuid NewGuid = FGuid::NewGuid();
	UniqueID.Append(TEXT("_"));
	UniqueID.Append(NewGuid.ToString());
	
	return UniqueID;
}

FString UDigumProgram::GetProgramVersion()
{
	// TODO dynamic program version
	return "base";
}
