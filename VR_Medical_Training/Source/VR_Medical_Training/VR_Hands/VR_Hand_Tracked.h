#pragma once

#include "CoreMinimal.h"
#include "HeadMountedDisplayTypes.h"
#include "VR_Hand.h"
#include "GameFramework/Actor.h"
#include "VR_Hand_Tracked.generated.h"

USTRUCT(BlueprintType)
struct FJointBoneMap
{
	GENERATED_BODY()

	FJointBoneMap()
	{
		
	}
	explicit FJointBoneMap(FName InBoneName)
	{
		BoneName = InBoneName;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EHandKeypoint Joint = EHandKeypoint::Palm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName BoneName = "Null";
};

UCLASS()
class VR_MEDICAL_TRAINING_API AVR_Hand_Tracked : public AVR_Hand
{
	GENERATED_BODY()
	
public:	
	AVR_Hand_Tracked();
	void InitializeJointData();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void PostLoad() override;
	void RecordJointTransforms(const FXRHandTrackingState& Data);
	
	UFUNCTION(BlueprintCallable, Category = "VR_Hands|Draw")
	void DrawJoints();
	UFUNCTION(BlueprintCallable, Category = "VR_Hands|Draw")
	void DrawJointsDebug();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Visual")
	TObjectPtr<UInstancedStaticMeshComponent> JointMeshInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR_Hands|Data|Joints")
	float JointScale = 0.05f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR_Hands|Data|Joints")
	TArray<FTransform> JointTransforms = TArray<FTransform>();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR_Hands|Data|Joints")
	TArray<int32> JointInstanceIndex = TArray<int32>();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Data|Joints")
	TArray<FJointBoneMap> JointBoneMaps = TArray<FJointBoneMap>();
	
	
private:
	FXRHandTrackingState TrackedHandData;
	int JointCount = 26;
};
