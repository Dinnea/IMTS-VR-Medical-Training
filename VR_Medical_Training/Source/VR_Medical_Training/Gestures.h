#pragma once

#include "CoreMinimal.h"

USTRUCT(BlueprintType)
struct FPose
{
	GENERATED_BODY()
	
	FPose()
	{
	}
	
	explicit FPose(const FString& InName) : Name(InName)
	{
	}
	
	FName Name = FName("");
};

USTRUCT(BlueprintType)
struct FPoseTransition
{
	GENERATED_BODY()
	
	FPoseTransition()
	{
	}
	
	explicit FPoseTransition(FPose InOldPose, FPose InNewPose) : OldPose(InOldPose), NewPose(InNewPose)
	{
	}
	
	FPose OldPose;
	FPose NewPose;
};

