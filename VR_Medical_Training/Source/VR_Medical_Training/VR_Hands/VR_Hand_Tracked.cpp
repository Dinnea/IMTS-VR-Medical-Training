#include "VR_Hand_Tracked.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/PoseableMeshComponent.h"

AVR_Hand_Tracked::AVR_Hand_Tracked()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("VR Hand Origin");
	
	JointMeshInstance = CreateDefaultSubobject<UInstancedStaticMeshComponent>("JointMeshInstance");
	JointMeshInstance->SetupAttachment(RootComponent);
	
	HandMesh = CreateDefaultSubobject<UPoseableMeshComponent>("HandMesh");
	HandMesh->SetupAttachment(RootComponent);
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
}

void AVR_Hand_Tracked::RegenerateJointBoneMaps()
{
	JointBoneMaps.Empty();	
	
	const FReferenceSkeleton& RefSkeleton = CachedMesh->GetRefSkeleton();	
	BonePool = RefSkeleton.GetRawRefBoneNames();
	
	UE_LOG(LogTemp, Warning, TEXT("Bone count is: %d"), BonePool.Num());
	
	for (int i = 0; i<JointCount; i++)
		JointBoneMaps.Add(FJointBoneMap(static_cast<EJoint>(i)));
}

void AVR_Hand_Tracked::InitializeJointData()
{
	for (int i = 0; i < JointCount; i++)
	{
		JointTransforms.Add(
			FTransform(FQuat::Identity, FVector::Zero(), FVector(JointScale)));
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

void AVR_Hand_Tracked::DrawJointCoordsDebug()
{
	const UWorld* World = GetWorld();
	
	for (int i = 0; i < JointCount; i++)
	{
		DrawDebugCoordinateSystem(World, 
			JointTransforms[i].GetLocation()+ LabelOffset,
			JointTransforms[i].Rotator(), 
			1);
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
	const FQuat RotationOffset = FQuat(0.5f, 0.5f, 0.5f, 0.5f);
		
	FTransform TransformOffset = FTransform();
	TransformOffset.SetRotation(RotationOffset);
	
	for (const FJointBoneMap JBMap : JointBoneMaps)
	{
		const int Index = static_cast<int>(JBMap.Joint);
		if (Index > 25)
			continue;
		
		
		FTransform ToUse = JointTransforms[Index]*TransformOffset;
		
		HandMesh->SetBoneTransformByName(JBMap.BoneName, ToUse, EBoneSpaces::WorldSpace);
	}
}

void AVR_Hand_Tracked::DrawJointMeshDebug()
{
	if (JointInstanceIndex.IsEmpty())
	{
		for (FTransform Transform : JointTransforms)
		{
			JointInstanceIndex.Add(JointMeshInstance->AddInstance(Transform, true));
		}
		
		return;
	}
	for (int i = 0; i < JointCount; i++)
	{
		JointMeshInstance->UpdateInstanceTransform(JointInstanceIndex[i], JointTransforms[i], true);
	}
}
