// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrabbableItem_Poseable.h"
#include "GrabbableScissors.generated.h"

/**
 * 
 */
UCLASS()
class VR_MEDICAL_TRAINING_API AGrabbableScissors : public AGrabbableItem_Poseable
{
	GENERATED_BODY()
	
public:
	virtual void Tick( float DeltaTime ) override;
};
