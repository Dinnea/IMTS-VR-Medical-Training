// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrabbableItem_Static.h"
#include "GrabbableDrawer.generated.h"

/**
 * 
 */
UCLASS()
class VR_MEDICAL_TRAINING_API AGrabbableDrawer : public AGrabbableItem_Static
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable)
	void SetMaxOffset(const float MaxOffset);
	virtual void Drop() override;
	
	UPROPERTY(BlueprintReadOnly)
	FVector TestLocation = FVector ::ZeroVector;
protected:
	virtual void BeginPlay() override;
	

private:
	float ConstraintMin;
	float ConstraintMax;
	
	FVector OriginalLocation;
	
};
