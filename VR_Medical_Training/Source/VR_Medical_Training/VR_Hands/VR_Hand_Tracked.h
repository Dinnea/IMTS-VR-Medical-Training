#pragma once

#include "CoreMinimal.h"
#include "HeadMountedDisplayTypes.h"
#include "VR_Hand.h"
#include "Components/PoseableMeshComponent.h"
#include "VR_Medical_Training/JointData.h"
#include "VR_Hand_Tracked.generated.h"

class UPoseableMeshComponent;
class UHandPoseRecognizer;
class UHandGestureRecognizer;
class AGrabbableItem;

struct FTipBinding
{
	EJoint Joint;
	TObjectPtr<USphereComponent> Collider;
};

UENUM(BlueprintType)
enum class EGrabMode : uint8
{
	Realistic,
	StickToHand	
};

UENUM()
enum EHandPose : uint8
{
	Grab,
	Pinch,
	None = 99
};

UCLASS()
class VR_MEDICAL_TRAINING_API AVR_Hand_Tracked : public AVR_Hand
{
	GENERATED_BODY()
	
public:
	void SetupComponents();
	void SetupColliders();
	AVR_Hand_Tracked();
	void InitializeJointData();
	virtual void Tick(float DeltaTime) override;
	virtual void PostLoad() override;
	
	FTransform GetJointTransform(EJoint Joint);
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	
	TObjectPtr<UPoseableMeshComponent> GetHandMesh(){return HandMesh;}
	
	UFUNCTION()
	TArray<FName> GetBonePool() {return BonePool;}
	

	UPROPERTY(BlueprintReadOnly, Category= "VR_Hands|Pose")
	float ScissorPinchStrength;	

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VR_Hands|Interaction")
	float PinchThreshold = 2; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VR_Hands|Interaction")
	float CurlThreshold = 60; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Interaction")
	float FingerTipColliderRadius = 1;
	
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Visual")
	TObjectPtr<UPoseableMeshComponent> HandMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Visual")
	TArray<FJointBoneMap> JointBoneMaps = TArray<FJointBoneMap>();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Visual")
	TObjectPtr<UInstancedStaticMeshComponent> JointMeshInstance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR_Hands|Data|Joints")
	TArray<FTransform> JointTransforms = TArray<FTransform>();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR_Hands|Data|Joints")
	TArray<int32> JointInstanceIndex = TArray<int32>();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Visual|Settings")
	bool bShowJointMeshDebug;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR_Hands|Visual|Settings")
	float DebugJointScale = 0.05f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Visual|Settings")
	bool bShowJointCoordsDebug;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR_Hands|Visual|Settings")
	float DebugCoordScale = 5.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Visual|Settings")
	bool bShowJointLabels;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Visual|Settings")
	FVector LabelOffset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Visual|Settings")
	bool bAnimateHand;
	
	
private:
	void RegenerateJointBoneMaps();
	void RecordJointTransforms();
	void ChangeBoneSuffix(const FString& From, const FString& To);
	void CorrectBoneNames();
	
	void DrawJointMeshDebug();
	void DrawJointCoordsDebug();
	void DrawJointNamesDebug();
	void AnimateHand();
	void GrabItem();
	void PinchItem();
	void DropItem();
	
	EHandPose CalculateHandPoses();
	
	bool GetFingerCurl(const FTransform& Distal, const FTransform& Intermediate, const FTransform& Proximal);
	bool ShouldDropItem();
	
	EHandPose HandPose = None;
	
	
	EGrabMode GrabMode = EGrabMode::Realistic;
	TArray<FTipBinding> FingerTipBindings;
	
	UPROPERTY() TObjectPtr<USceneComponent> ColliderParent;
	
	UPROPERTY(VisibleAnywhere)	TObjectPtr<USphereComponent> ThumbTipCollider;
	UPROPERTY()	TObjectPtr<USphereComponent> IndexTipCollider;
	UPROPERTY()	TObjectPtr<USphereComponent> MiddleTipCollider;
	UPROPERTY()	TObjectPtr<USphereComponent> RingTipCollider;
	UPROPERTY()	TObjectPtr<USphereComponent> PinkieTipCollider;
	
	UPROPERTY()
	TObjectPtr<USkinnedAsset> CachedMesh = nullptr;
	
	UPROPERTY()
	TArray<FName> BonePool;
	
	UPROPERTY()
	TObjectPtr<AGrabbableItem> Grabbed = nullptr;
	
	FXRHandTrackingState TrackedHandData;
	int JointCount = 26;
	
	
	float GetAngleDegrees(FVector A, FVector B)
	{
		A.Normalize();
		B.Normalize();

		float Dot = FVector::DotProduct(A, B);
		Dot = FMath::Clamp(Dot, -1.0f, 1.0f);

		return FMath::RadiansToDegrees(FMath::Acos(Dot));
	}
};
