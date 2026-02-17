#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VR_Hand_Controller.generated.h"

class UMotionControllerComponent;
class UWidgetInteractionComponent;
class USphereComponent;

UCLASS()
class VR_MEDICAL_TRAINING_API AVR_Hand_Controller : public AActor
{
	GENERATED_BODY()
	
public:	
	AVR_Hand_Controller();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	
// Components
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UMotionControllerComponent> MotionController;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Hands")
	TObjectPtr<USkeletalMeshComponent> HandMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Hands")
	TObjectPtr<UWidgetInteractionComponent> WidgetInteractionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Hands")
	TObjectPtr<USphereComponent> GrabShpere;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|Hands|HandData")
	EControllerHand HandType;
	
	UPROPERTY(BlueprintReadOnly, Category = "Components|Hands|HandData")
	bool bIsAnimationMirrored;
};
