#pragma once

#include "CoreMinimal.h"
#include "HeadMountedDisplayTypes.h"
#include "VR_Hand.h"
#include "VR_Medical_Training/Gestures.h"
#include "VR_Medical_Training/JointData.h"
#include "VR_Hand_Tracked.generated.h"

class UHandPoseLibrary;
struct FPoseTransition;
class UPoseableMeshComponent;
class UHandPoseRecognizer;
class UHandGestureRecognizer;
class AGrababbleItem;

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
	void SetupComponents();
	void SetupColliders();
	void SetUpPoseDatabase();
	AVR_Hand_Tracked();
	void InitializeJointData();
	virtual void Tick(float DeltaTime) override;
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	UFUNCTION(BlueprintCallable)
	void RegisterPose() const;

	UFUNCTION()
	TArray<FName> GetBonePool() {return BonePool;}
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPoseTransition, const FPoseTransition&);
	FOnPoseTransition OnPoseTransition;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Tracking")
	TObjectPtr<UHandPoseRecognizer> PoseRecognizer;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Tracking")
	TObjectPtr<UHandGestureRecognizer> GestureRecognizer;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VR_Hands|Tracking")
	TObjectPtr<UHandPoseLibrary> HandPoseLibrary;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR_Hands|Physics")
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
	void DropItem();

	void HandlePoseTransition(const FPoseTransition& PoseTransition);
	
	TArray<FTipBinding> FingerTipBindings;
	
	UPROPERTY() TObjectPtr<USceneComponent> ColliderParent;
	
	UPROPERTY()	TObjectPtr<USphereComponent> ThumbTipCollider;
	UPROPERTY()	TObjectPtr<USphereComponent> IndexTipCollider;
	UPROPERTY()	TObjectPtr<USphereComponent> MiddleTipCollider;
	UPROPERTY()	TObjectPtr<USphereComponent> RingTipCollider;
	UPROPERTY()	TObjectPtr<USphereComponent> PinkieTipCollider;
	
	UPROPERTY()
	TObjectPtr<USkinnedAsset> CachedMesh = nullptr;
	
	UPROPERTY()
	TArray<FName> BonePool;
	
	UPROPERTY()
	TObjectPtr<AGrababbleItem> Grabbed = nullptr;
	
	FPose CurrentPose;
	
	FXRHandTrackingState TrackedHandData;
	int JointCount = 26;
};
