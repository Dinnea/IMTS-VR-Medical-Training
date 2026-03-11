#pragma once

#include "CoreMinimal.h"
#include "HeadMountedDisplayTypes.h"
#include "VR_Hand.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Actor.h"
#include "VR_Hand_Tracked.generated.h"

UENUM(BlueprintType)
enum class EJoint : uint8
{
	Palm,
	Wrist,
	ThumbMetacarpal,
	ThumbProximal,
	ThumbDistal,
	ThumbTip,
	IndexMetacarpal,
	IndexProximal,
	IndexIntermediate,
	IndexDistal,
	IndexTip,
	MiddleMetacarpal,
	MiddleProximal,
	MiddleIntermediate,
	MiddleDistal,
	MiddleTip,
	RingMetacarpal,
	RingProximal,
	RingIntermediate,
	RingDistal,
	RingTip,
	LittleMetacarpal,
	LittleProximal,
	LittleIntermediate,
	LittleDistal,
	LittleTip,
	None = 99
};

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
	EJoint Joint = EJoint::None;

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
	
	UFUNCTION(BlueprintCallable, Category = "VR_Hands|Debug")
	void DrawJoints();
	UFUNCTION(BlueprintCallable, Category = "VR_Hands|Debug")
	void DrawJointsDebug();
	UFUNCTION(BlueprintCallable, Category = "VR_Hands|Debug")
	void DrawJointNamesDebug();
	UFUNCTION(BlueprintCallable, Category = "VR_Hands|Animate")
	void AnimateHands();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR_Hands|Visual")
	TObjectPtr<UPoseableMeshComponent> HandMesh;
	
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

	UPROPERTY()
	USkinnedAsset* CachedMesh = nullptr;
};
