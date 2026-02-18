#pragma once

#include "CoreMinimal.h"
#include "HeadMountedDisplayTypes.h"
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
	void RecordJointTransforms(const FXRHandTrackingState& Data);
	void DrawHand();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Visual")
	TObjectPtr<UStaticMesh> JointMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Data|Joints")
	FVector JointScale = FVector(0.01f, 0.01f, 0.01f);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR_Hands|Data|Joints")
	TArray<FTransform> JointTransforms = TArray<FTransform>();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR_Hands|Data|Joints")
	TArray<UStaticMeshComponent*> JointMeshes;
	
private:
	FXRHandTrackingState TrackedHandData;
};
