#pragma once

#include "CoreMinimal.h"
#include "VR_Hand.h"
#include "GameFramework/Actor.h"
#include "VR_Hand_Controller.generated.h"

class UMotionControllerComponent;

UCLASS()
class VR_MEDICAL_TRAINING_API AVR_Hand_Controller : public AVR_Hand
{
	GENERATED_BODY()
	
public:	
	AVR_Hand_Controller();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	
// Components
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR_Hands")
	TObjectPtr<UMotionControllerComponent> MotionController;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR_Hands|Visual")
	TObjectPtr<USkeletalMeshComponent> HandMesh;
	UPROPERTY(BlueprintReadOnly, Category = "VR_Hands|HandData")
	bool bIsAnimationMirrored;
};
