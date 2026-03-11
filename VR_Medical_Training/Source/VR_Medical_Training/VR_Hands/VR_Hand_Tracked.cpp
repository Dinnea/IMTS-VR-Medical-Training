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
	USkinnedAsset* CurrentMesh = HandMesh->GetSkinnedAsset();
	
	if (CurrentMesh == CachedMesh)
		return;
	
	CachedMesh = CurrentMesh;
	
	JointBoneMaps.Empty();
	if (!HandMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("HandMesh component is null"));
		return;;
	}
	
	if (!CurrentMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("No mesh assigned yet"));
		return;
	}
	
	const FReferenceSkeleton& refSkeleton = CurrentMesh->GetRefSkeleton();	
	TArray<FName> BoneNames = refSkeleton.GetRawRefBoneNames();
	
	UE_LOG(LogTemp, Warning, TEXT("Bone count is: %d"), BoneNames.Num());
	
	for (const FName Bone : BoneNames)
	{
		// const bool bBoneExists = JointBoneMaps.ContainsByPredicate(
		// 	[&](const FJointBoneMap& Entry)
		// 	{
		// 		return Entry.BoneName == Bone;
		// 	});
		//
		// if (!bBoneExists)
			JointBoneMaps.Add(FJointBoneMap(Bone));
		UE_LOG(LogTemp, Warning, TEXT("Bones added: %d"), JointBoneMaps.Num());
	}
}

void AVR_Hand_Tracked::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UHeadMountedDisplayFunctionLibrary::GetHandTrackingState(this, EXRSpaceType::UnrealWorldSpace, 
		HandType, TrackedHandData);
	
	if (!TrackedHandData.bValid)
	{
		UE_LOG(LogTemp, Warning,
				TEXT("Hand tracking data received is invalid / empty, hands will not be drawn."));
		return;
	}
	
	RecordJointTransforms(TrackedHandData);
}

void AVR_Hand_Tracked::InitializeJointData()
{
	for (int i = 0; i < JointCount; i++)
	{
		JointTransforms.Add(
			FTransform(FQuat::Identity, FVector::Zero(), FVector(JointScale)));
	}
}

void AVR_Hand_Tracked::RecordJointTransforms(const FXRHandTrackingState& Data)
{	
	const TArray<FVector>& JointsLocations = Data.HandKeyLocations;
	const TArray<FQuat>& JointsRotations = Data.HandKeyRotations;
	
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

void AVR_Hand_Tracked::DrawJointsDebug()
{
	const UWorld* World = GetWorld();
	
	for (int i = 0; i < JointCount; i++)
	{
		DrawDebugCoordinateSystem(World, 
			JointTransforms[i].GetLocation(), 
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

void AVR_Hand_Tracked::AnimateHands()
{
	for (const FJointBoneMap JBMap : JointBoneMaps)
	{
		const int Index = static_cast<int>(JBMap.Joint);
		if (Index > 25)
			continue;
		
		HandMesh->SetBoneTransformByName(JBMap.BoneName, JointTransforms[Index], EBoneSpaces::WorldSpace);
	}
}

void AVR_Hand_Tracked::DrawJoints()
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
