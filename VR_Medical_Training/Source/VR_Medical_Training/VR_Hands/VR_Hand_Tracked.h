#pragma once

#include "CoreMinimal.h"
#include "HeadMountedDisplayTypes.h"
#include "VR_Hand.h"
#include "VR_Medical_Training/JointData.h"
#include "VR_Hand_Tracked.generated.h"

struct FPoseTransition;
class UPoseableMeshComponent;
class UHandPoseRecognizer;
class UHandGestureRecognizer;

struct FTipBinding
{
	EJoint Joint;
	TObjectPtr<USphereComponent> Collider;
};

UCLASS()
class VR_MEDICAL_TRAINING_API AVR_Hand_Tracked : public AVR_Hand
{
	GENERATED_BODY()
	
public:	
	AVR_Hand_Tracked();
	void InitializeJointData();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	TArray<FName> GetBonePool() {return BonePool;}
	
	DECLARE_MULTICAST_DELEGATE_OneParam(OnPoseTransition, const FPoseTransition&);

protected:
	virtual void BeginPlay() override;
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR_Hands|Tracking")
	TObjectPtr<UHandPoseRecognizer> PoseRecognizer;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR_Hands|Tracking")
	TObjectPtr<UHandGestureRecognizer> GestureRecognizer;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Physics")
	float FingerTipColliderRadius = 1;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR_Hands|Physics")
	TArray<TObjectPtr<USphereComponent>> FingertipColliders;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR_Hands|Visual")
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
	
	void HandlePoseTransition(FPoseTransition& PoseTransition);
	
	const TArray<FTipBinding> FingerTipBindings =
	{
		{ EJoint::ThumbTip,  ThumbTipCollider },
		{ EJoint::IndexTip,  IndexTipCollider },
		{ EJoint::MiddleTip, MiddleTipCollider },
		{ EJoint::RingTip,   RingTipCollider },
		{ EJoint::LittleTip, PinkieTipCollider }
	};
	
	UPROPERTY() TObjectPtr<USceneComponent> ColliderParent;
	
	UPROPERTY()	TObjectPtr<USphereComponent> ThumbTipCollider;
	UPROPERTY()	TObjectPtr<USphereComponent> IndexTipCollider;
	UPROPERTY()	TObjectPtr<USphereComponent> MiddleTipCollider;
	UPROPERTY()	TObjectPtr<USphereComponent> RingTipCollider;
	UPROPERTY()	TObjectPtr<USphereComponent> PinkieTipCollider;
	
	UPROPERTY()
	USkinnedAsset* CachedMesh = nullptr;
	
	UPROPERTY()
	TArray<FName> BonePool;
	
	FXRHandTrackingState TrackedHandData;
	int JointCount = 26;
};
