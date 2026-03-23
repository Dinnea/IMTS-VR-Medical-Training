#include "VR_Hand_Tracked.h"

#include "HandGestureRecognizer.h"
#include "HandPoseRecognizer.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "SphereComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/PoseableMeshComponent.h"

AVR_Hand_Tracked::AVR_Hand_Tracked()
{
	PrimaryActorTick.bCanEverTick = true;
		
	HandMesh = CreateDefaultSubobject<UPoseableMeshComponent>("HandMesh");
	RootComponent = HandMesh;
	
	JointMeshInstance = CreateDefaultSubobject<UInstancedStaticMeshComponent>("JointMeshInstance");
	JointMeshInstance->SetupAttachment(RootComponent);
	
	PoseRecognizer = CreateDefaultSubobject<UHandPoseRecognizer>("PoseRecognizer");
	PoseRecognizer->SetupAttachment(RootComponent);
	
	GestureRecognizer = CreateDefaultSubobject<UHandGestureRecognizer>("GestureRecognizer");
	GestureRecognizer->SetupAttachment(RootComponent);
	
	ColliderParent = CreateDefaultSubobject<USceneComponent>("Colliders");
	ColliderParent->SetupAttachment(RootComponent);
	
	ThumbTipCollider =  CreateDefaultSubobject<USphereComponent>("ThumbTipCollider");
	IndexTipCollider = CreateDefaultSubobject<USphereComponent>("IndexTipCollider");
	MiddleTipCollider = CreateDefaultSubobject<USphereComponent>("MiddleTipCollider");
	RingTipCollider = CreateDefaultSubobject<USphereComponent>("RingTipCollider");
	PinkieTipCollider = CreateDefaultSubobject<USphereComponent>("LittleTipCollider");
	
	FingertipColliders.Add(ThumbTipCollider);
	FingertipColliders.Add(IndexTipCollider);
	FingertipColliders.Add(MiddleTipCollider);
	FingertipColliders.Add(RingTipCollider);
	FingertipColliders.Add(PinkieTipCollider);
	
	for (const auto FingertipCollider : FingertipColliders)
	{
		FingertipCollider->SetupAttachment(ColliderParent);
		FingertipCollider->SetSphereRadius(FingerTipColliderRadius);
	}
}

void AVR_Hand_Tracked::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeJointData();
}


void AVR_Hand_Tracked::PostLoad()
{
	Super::PostLoad();
	
	// Check current mesh
	USkinnedAsset* CurrentMesh = HandMesh->GetSkinnedAsset();
	UE_LOG(LogTemp, Warning, TEXT("is empty? %s"), JointBoneMaps.IsEmpty()? TEXT("TRUE") : TEXT("FALSE"));
	
	if (CurrentMesh == CachedMesh && !JointBoneMaps.IsEmpty())
		return;
	
	if (!CurrentMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("No mesh assigned yet"));
		return;
	}
	
	CachedMesh = CurrentMesh;
	
	RegenerateJointBoneMaps();
}

void AVR_Hand_Tracked::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const auto ChangedProperty = PropertyChangedEvent.GetMemberPropertyName();
	if (ChangedProperty == GET_MEMBER_NAME_CHECKED(AVR_Hand_Tracked, JointBoneMaps))
	{
		CorrectBoneNames();
	}
	if (ChangedProperty == GET_MEMBER_NAME_CHECKED(AVR_Hand_Tracked, FingerTipColliderRadius))
	{
		for (const auto FingertipCollider : FingertipColliders)
			FingertipCollider->SetSphereRadius(FingerTipColliderRadius);
	}
}

void AVR_Hand_Tracked::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RecordJointTransforms();
	
	if (bShowJointMeshDebug)
		DrawJointMeshDebug();
	
	if (bShowJointCoordsDebug)
		DrawJointCoordsDebug();
	
	if (bShowJointLabels)
		DrawJointNamesDebug();
	
	if (bAnimateHand)
		AnimateHand();
	
	for (const auto& [Joint, Collider] : FingerTipBindings)
	{
		const uint8 JointTipIndex = static_cast<uint8>(Joint);
		Collider->SetWorldTransform((JointTransforms[JointTipIndex]));
	}
}

void AVR_Hand_Tracked::RegenerateJointBoneMaps()
{
	JointBoneMaps.Empty();	
	
	const FReferenceSkeleton& RefSkeleton = CachedMesh->GetRefSkeleton();	
	BonePool = RefSkeleton.GetRawRefBoneNames();
	BonePool.Add(NAME_None);
	
	UE_LOG(LogTemp, Warning, TEXT("Bone count is: %d"), BonePool.Num());
	
	for (int i = 0; i<JointCount; i++)
		JointBoneMaps.Add(FJointBoneMap(static_cast<EJoint>(i)));
}

void AVR_Hand_Tracked::InitializeJointData()
{
	for (int i = 0; i < JointCount; i++)
	{
		JointTransforms.Add(
			FTransform(FQuat::Identity, FVector::Zero()));
	}
}

void AVR_Hand_Tracked::RecordJointTransforms()
{	
	UHeadMountedDisplayFunctionLibrary::GetHandTrackingState(this, EXRSpaceType::UnrealWorldSpace, 
		HandType, TrackedHandData);
	
	if (!TrackedHandData.bValid)
	{
		UE_LOG(LogTemp, Warning,
				TEXT("Hand tracking data received is invalid / empty, hands will not be drawn."));
		return;
	}
	
	const TArray<FVector>& JointsLocations = TrackedHandData.HandKeyLocations;
	const TArray<FQuat>& JointsRotations = TrackedHandData.HandKeyRotations;
	
	if (JointsLocations.Num() != JointCount || JointsRotations.Num() != JointCount)
	{
		if (GEngine) 
			GEngine->AddOnScreenDebugMessage(-1, 4.0, FColor::Red, 
			FString::Printf(TEXT("ERROR: Joint number is %i, when it should be 26"), JointsLocations.Num()));
		
		UE_LOG(LogTemp, Warning,
				TEXT("Joint number is %i, when it should be 26, position will not be updated."), JointsLocations.Num());
	}
	
	for (int i = 0; i < JointCount; i++)
	{
		JointTransforms[i].SetRotation(JointsRotations[i]);
		JointTransforms[i].SetLocation(JointsLocations[i]);
	}
}

void AVR_Hand_Tracked::ChangeBoneSuffix(const FString& From, const FString& To)
{
	if (HandType != EControllerHand::Left && HandType != EControllerHand::Right)
		return;
	
	for (auto& JointBoneMap : JointBoneMaps)
	{
		if (FString NewName = JointBoneMap.BoneName.ToString(); NewName.RemoveFromEnd(From))
		{		
			NewName += To;
			JointBoneMap.BoneName = FName(NewName);
		}
	}
}

void AVR_Hand_Tracked::CorrectBoneNames()
{
	const FString LeftSuffix = "l";
	const FString RightSuffix = "r";
	
	const FString& From = (HandType == EControllerHand::Left) ? RightSuffix : LeftSuffix;
	const FString& To   = (HandType == EControllerHand::Left) ? LeftSuffix  : RightSuffix;

	ChangeBoneSuffix(From, To);
}

void AVR_Hand_Tracked::DrawJointCoordsDebug()
{
	const UWorld* World = GetWorld();
	
	for (int i = 0; i < JointCount; i++)
	{
		DrawDebugCoordinateSystem(World, 
			JointTransforms[i].GetLocation()+ LabelOffset,
			JointTransforms[i].Rotator(), 
			DebugCoordScale);
	}
}

void AVR_Hand_Tracked::DrawJointNamesDebug()
{
	const UWorld* World = GetWorld();
	
	for (int i = 0; i < JointCount; i++)
	{
		FString JointName = StaticEnum<EHandKeypoint>()->GetNameByValue(i).ToString();
		
		DrawDebugString(World, 
			JointTransforms[i].GetLocation(), 
			JointName, 
			nullptr, 
			FColor::Green, 
			0.f, 
			true);
	}
}

void AVR_Hand_Tracked::AnimateHand()
{
	const FQuat RotationOffset = FRotator(0.f, 90.f, 90.f).Quaternion();;
		
	FTransform TransformOffset = FTransform();
	TransformOffset.SetRotation(RotationOffset);
	
	for (const FJointBoneMap JBMap : JointBoneMaps)
	{
		const int Index = static_cast<int>(JBMap.Joint);
		if (Index > 25)
			continue;
		
		FTransform ToUse = TransformOffset * JointTransforms[Index];
		
		HandMesh->SetBoneTransformByName(JBMap.BoneName, ToUse, EBoneSpaces::WorldSpace);
	}
}

void AVR_Hand_Tracked::HandlePoseTransition(FPoseTransition& PoseTransition)
{
	//if is grab, then Grab()
	
	//if grab is over, then Drop()
}

void AVR_Hand_Tracked::DrawJointMeshDebug()
{
	if (JointInstanceIndex.IsEmpty())
	{
		for (FTransform Transform : JointTransforms)
		{
			FTransform ToUse = Transform;
			ToUse.SetScale3D(FVector(DebugJointScale)); 
			JointInstanceIndex.Add(JointMeshInstance->AddInstance(Transform, true));
		}
		
		return;
	}
	for (int i = 0; i < JointCount; i++)
	{
		JointMeshInstance->UpdateInstanceTransform(JointInstanceIndex[i], JointTransforms[i], true);
	}
}
