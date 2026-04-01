// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandPose.h"
#include "Engine/DataAsset.h"
#include "HandPoseLibrary.generated.h"

/**
 * 
 */
UCLASS()
class VR_MEDICAL_TRAINING_API UHandPoseLibrary : public UDataAsset
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(EditAnywhere, Category="Hand Poses")
	TArray<FHandPose> Poses;
};
