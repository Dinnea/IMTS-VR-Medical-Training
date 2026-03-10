#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VR_Hand.generated.h"

class UWidgetInteractionComponent;
class USphereComponent;

UCLASS()
class VR_MEDICAL_TRAINING_API AVR_Hand : public AActor
{
	GENERATED_BODY()
	
public:	
	AVR_Hand();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR_Hands")
	TObjectPtr<UWidgetInteractionComponent> WidgetInteractionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR_Hands|Visual")
	TObjectPtr<USkeletalMeshComponent> HandMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR_Hands")
	TObjectPtr<USphereComponent> GrabSphere;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|HandData")
	EControllerHand HandType;
};
