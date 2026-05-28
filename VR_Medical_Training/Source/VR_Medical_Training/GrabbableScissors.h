// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrabbableItem_Poseable.h"
#include "GrabbableScissors.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VR_MEDICAL_TRAINING_API AGrabbableScissors : public AGrabbableItem_Poseable
{
	GENERATED_BODY()
	
public:
	AGrabbableScissors();
	virtual void Tick( float DeltaTime ) override;
};
