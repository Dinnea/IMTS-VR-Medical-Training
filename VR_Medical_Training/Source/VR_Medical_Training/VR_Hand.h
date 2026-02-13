#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionControllerComponent.h"
#include "VR_Hand.generated.h"

UCLASS()
class VR_MEDICAL_TRAINING_API AVR_Hand : public AActor
{
	GENERATED_BODY()
	
public:	
	AVR_Hand();

protected:
	virtual void BeginPlay() override;

	UMotionControllerComponent* motionController;

public:	
	virtual void Tick(float DeltaTime) override;

};
