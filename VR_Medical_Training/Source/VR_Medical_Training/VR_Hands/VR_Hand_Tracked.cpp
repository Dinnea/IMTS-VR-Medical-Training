#include "VR_Hand_Tracked.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SphereComponent.h"
#include "VR_Medical_Training/GrabbableItem.h"
#include "VR_Medical_Training/GrabbableScissors.h"

AVR_Hand_Tracked::AVR_Hand_Tracked()
{
	PrimaryActorTick.bCanEverTick = true;
		
	SetupComponents();
	
	SetupColliders();
}

void AVR_Hand_Tracked::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeJointData();
	
	Grabbed = nullptr;
}


void AVR_Hand_Tracked::PostLoad()
{
	Super::PostLoad();
	
	RegenerateJointBoneMaps();
}

FTransform AVR_Hand_Tracked::GetJointTransform(EJoint Joint)
{
	return JointTransforms[static_cast<int>(Joint)];
}
#if WITH_EDITOR
void AVR_Hand_Tracked::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const auto ChangedProperty = PropertyChangedEvent.GetMemberPropertyName();
	// if (ChangedProperty == GET_MEMBER_NAME_CHECKED(AVR_Hand_Tracked, JointBoneMaps))
	// {
	// 	//CorrectBoneNames();
	// }
	//
	// if (ChangedProperty == GET_MEMBER_NAME_CHECKED(AVR_Hand_Tracked, HandMesh->SkeletalMesh))
	// {
	// 	RegenerateJointBoneMaps();
	// }
	
	if (ChangedProperty == GET_MEMBER_NAME_CHECKED(AVR_Hand_Tracked, FingerTipColliderRadius))
	{
		for (const auto [Joint, Collider] : FingerTipBindings)
			Collider->SetSphereRadius(FingerTipColliderRadius);
	}
}
#endif


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
	
	if (IsGrabbing) GrabItem();
	else if (ShouldDropItem()) DropItem();
	
	if (Grabbed && Grabbed->IsInSpawn)
			UE_LOG(LogTemp, Warning, TEXT("IsInSpawn true"));
	
	CalculateHandPoses();
	
	if (IsGrabbing)  { UE_LOG(LogTemp, Warning, TEXT("IsGrabbing: true")); }
	else UE_LOG(LogTemp, Warning, TEXT("IsGrabbing: false"));
	
	if (IsPinched)  { UE_LOG(LogTemp, Warning, TEXT("IsPinched: true")); }
	else UE_LOG(LogTemp, Warning, TEXT("IsPinched: false"));
	
	if (IsHandCurled)  { UE_LOG(LogTemp, Warning, TEXT("IsHandCurled: true")); }
	else UE_LOG(LogTemp, Warning, TEXT("IsHandCurled: false"));
}

bool AVR_Hand_Tracked::ShouldDropItem()
{
	switch (GrabMode)
	{
	case EGrabMode::Realistic:
		return !IsGrabbing && Grabbed != nullptr;
		
	case EGrabMode::StickToHand:
		if (Grabbed)
			return !IsGrabbing && Grabbed->IsInSpawn;
		break;
	}
	
	return false;
}

void AVR_Hand_Tracked::SetupComponents()
{
	HandMesh = CreateDefaultSubobject<UPoseableMeshComponent>("HandMesh");
	RootComponent = HandMesh;
	
	JointMeshInstance = CreateDefaultSubobject<UInstancedStaticMeshComponent>("JointMeshInstance");
	JointMeshInstance->SetupAttachment(RootComponent);
	
	ColliderParent = CreateDefaultSubobject<USceneComponent>("Colliders");
	ColliderParent->SetupAttachment(RootComponent);
}

void AVR_Hand_Tracked::SetupColliders()
{
	ThumbTipCollider =  CreateDefaultSubobject<USphereComponent>("ThumbTipCollider");
	IndexTipCollider = CreateDefaultSubobject<USphereComponent>("IndexTipCollider");
	MiddleTipCollider = CreateDefaultSubobject<USphereComponent>("MiddleTipCollider");
	RingTipCollider = CreateDefaultSubobject<USphereComponent>("RingTipCollider");
	PinkieTipCollider = CreateDefaultSubobject<USphereComponent>("LittleTipCollider");
	
	FingerTipBindings=
	{
		{ EJoint::ThumbTip,  ThumbTipCollider },
		{ EJoint::IndexTip,  IndexTipCollider },
		{ EJoint::MiddleTip, MiddleTipCollider },
		{ EJoint::RingTip,   RingTipCollider },
		{ EJoint::LittleTip, PinkieTipCollider }
	};
	
	for (const auto [Joint, Collider] : FingerTipBindings)
	{
		Collider->SetupAttachment(ColliderParent);
		Collider->SetSphereRadius(FingerTipColliderRadius);
	}
}

void AVR_Hand_Tracked::SwitchGrabMode()
{
	GrabMode = GrabMode == EGrabMode::StickToHand ? EGrabMode::Realistic : EGrabMode::StickToHand;
}

void AVR_Hand_Tracked::RegenerateJointBoneMaps()
{
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
	JointBoneMaps.Empty();	
	
	const FReferenceSkeleton& RefSkeleton = CachedMesh->GetRefSkeleton();	
	BonePool = RefSkeleton.GetRawRefBoneNames();
	BonePool.Add(NAME_None);
	
	UE_LOG(LogTemp, Warning, TEXT("Bone count is: %d"), BonePool.Num());
	
	for (int i = 0; i<JointCount; i++)
	{
		JointBoneMaps.Add(FJointBoneMap(static_cast<EJoint>(i)));

	FString JointName = EJointToString(i);
		
		for (auto BoneName : BonePool)
		{
			if (!BoneName.ToString().Contains(JointName))
				continue;
			
			JointBoneMaps.Last().BoneName = BoneName;
		}
	}
	
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
		// UE_LOG(LogTemp, Warning,
		// 		TEXT("Hand tracking data received is invalid / empty, hands will not be drawn."));
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
	
	for (const auto& [Joint, Collider] : FingerTipBindings)
	{
		const uint8 JointTipIndex = static_cast<uint8>(Joint);
		if (Collider == nullptr) UE_LOG(LogTemp, Warning, TEXT("NULL COLLIDER"));
		Collider->SetWorldTransform((JointTransforms[JointTipIndex]));
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

void AVR_Hand_Tracked::GrabItem()
{
	if (Grabbed!=nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Already holding actor"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("GrabItem()"));
	
	TArray<AActor*> OverlappingActors;
	IndexTipCollider -> GetOverlappingActors(OverlappingActors);
	
	
	for (auto* Actor : OverlappingActors)
	{
		if (!Actor || Actor == this) continue;
		
		auto* GrabbedActor = Cast<AGrabbableItem>(Actor);
		if (!GrabbedActor) continue;
		
		GrabbedActor->Grab(this);
		Grabbed = GrabbedActor;
		
		FName SocketName = "Socket_PinchHold";
		
		if (auto* Scissors = Cast<AGrabbableScissors>(GrabbedActor))
		{
			GrabMode = EGrabMode::StickToHand;
		}
		
		else
		{
			GrabbedActor->AttachToComponent(HandMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			UE_LOG(LogTemp, Warning, TEXT("pinch hold"));
		}
	}
}

void AVR_Hand_Tracked::DropItem()
{
	UE_LOG(LogTemp, Warning, TEXT("Drop item"));
	
	Grabbed->Drop();
	
	if (Grabbed->ObjectName == "Scissors")
			GrabMode = EGrabMode::Realistic;
		
	Grabbed = nullptr;
}

void AVR_Hand_Tracked::CalculateHandPoses()
{
	const FTransform ThumbTip =  JointTransforms[static_cast<int>(EHandKeypoint::ThumbTip)];
	const FTransform IndexTip = JointTransforms[static_cast<int>(EHandKeypoint::IndexTip)];
	const FTransform MiddleTip = JointTransforms[static_cast<int>(EHandKeypoint::MiddleTip)];
	const FTransform RingTip = JointTransforms[static_cast<int>(EHandKeypoint::RingTip)];

	
	const FTransform ThumbDistal = JointTransforms[static_cast<int>(EHandKeypoint::ThumbDistal)];
	const FTransform IndexDistal = JointTransforms[static_cast<int>(EHandKeypoint::IndexDistal)];
	const FTransform MiddleDistal = JointTransforms[static_cast<int>(EHandKeypoint::MiddleDistal)];
	const FTransform RingDistal = JointTransforms[static_cast<int>(EHandKeypoint::RingDistal)];
	
	const FTransform IndexIntermediate = JointTransforms[static_cast<int>(EHandKeypoint::IndexIntermediate)];
	const FTransform MiddleIntermediate = JointTransforms[static_cast<int>(EHandKeypoint::MiddleIntermediate)];
	const FTransform RingIntermediate = JointTransforms[static_cast<int>(EHandKeypoint::RingIntermediate)];

	const FTransform IndexProximal = JointTransforms[static_cast<int>(EHandKeypoint::IndexProximal)];
	const FTransform MiddleProximal = JointTransforms[static_cast<int>(EHandKeypoint::MiddleProximal)];
	const FTransform RingProximal = JointTransforms[static_cast<int>(EHandKeypoint::RingProximal)];
	
	
	const float	PinchStrength = FVector::Dist(ThumbTip.GetLocation(), IndexTip.GetLocation());
	IsPinched = PinchStrength <= PinchThreshold;
	
	ScissorPinchStrength = FVector::Dist (IndexDistal.GetLocation(), ThumbDistal.GetLocation());
	
	if (GetFingerCurl(IndexDistal, IndexIntermediate, IndexProximal) &&
		GetFingerCurl(MiddleDistal, MiddleIntermediate, MiddleProximal) &&
		GetFingerCurl(RingDistal, RingIntermediate, RingProximal)) IsHandCurled = true;
	else IsHandCurled = false;
	
	IsGrabbing = IsPinched || IsHandCurled;
}

bool AVR_Hand_Tracked::GetFingerCurl(const FTransform& Distal, const FTransform& Intermediate, const FTransform& Proximal)
{
	const FVector DistRot = Distal.Rotator().Vector();
	const FVector IntRot = Intermediate.Rotator().Vector();
	const FVector ProxRot = Proximal.Rotator().Vector();

	const float DistIntAngle = GetAngleDegrees(DistRot, IntRot);
	const float IntProxAngle = GetAngleDegrees(IntRot, ProxRot);
	
	if ((DistIntAngle + IntProxAngle) > CurlThreshold) return true;
	
	return false;
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
		JointMeshInstance->UpdateInstanceTransform(JointInstanceIndex[i], JointTransforms[i], true);
}
