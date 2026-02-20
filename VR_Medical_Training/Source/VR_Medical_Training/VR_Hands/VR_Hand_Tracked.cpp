#include "VR_Hand_Tracked.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/InstancedStaticMeshComponent.h"

AVR_Hand_Tracked::AVR_Hand_Tracked()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("VR Hand Origin");
	JointMeshInstance = CreateDefaultSubobject<UInstancedStaticMeshComponent>("JointMeshInstance");
	JointMeshInstance->SetupAttachment(RootComponent);
}

void AVR_Hand_Tracked::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeJointData();
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
		DrawDebugCoordinateSystem(World, JointTransforms[i].GetLocation(), JointTransforms[i].Rotator(), 1);
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
