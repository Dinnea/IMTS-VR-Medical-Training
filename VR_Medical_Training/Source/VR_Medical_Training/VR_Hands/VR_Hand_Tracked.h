#pragma once

#include "CoreMinimal.h"
#include "VR_Hand.h"
#include "GameFramework/Actor.h"
#include "VR_Hand_Tracked.generated.h"

UCLASS()
class VR_MEDICAL_TRAINING_API AVR_Hand_Tracked : public AVR_Hand
{
	GENERATED_BODY()
	
public:	
	AVR_Hand_Tracked();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

};
