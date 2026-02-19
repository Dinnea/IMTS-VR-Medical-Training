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
}

void AVR_Hand_Tracked::RecordJointTransforms(const FXRHandTrackingState& Data)
{	
	JointTransforms.Empty();
	
	const TArray<FVector>& JointsLocations = Data.HandKeyLocations;
	const TArray<FQuat>& JointsRotations = Data.HandKeyRotations;
	
	if (JointsLocations.Num() != 26 || JointsRotations.Num() != 26)
	//missing joints / too many joints
	{
		if (GEngine) 
			GEngine->AddOnScreenDebugMessage(-1, 4.0, FColor::Red, 
			FString::Printf(TEXT("Joint nr %i"), JointsLocations.Num()));
		return;
	}
	
	for (int i = 0; i < 26; i++)
	{
		JointTransforms.Add(
			FTransform(JointsRotations[i],JointsLocations[i], FVector(JointScale)));
	}
}

void AVR_Hand_Tracked::DrawHand()
{
	if (JointInstanceIndex.IsEmpty())
	{
		for (int i = 0; i < 26; i++)
		{
			//const FName JointName(TEXT("JointMeshInstance"), i);
			JointInstanceIndex.Add(JointMeshInstance->AddInstance(JointTransforms[i], true));
		}
		
		return;
	}
	for (int i = 0; i < 26; i++)
	{
		JointMeshInstance->UpdateInstanceTransform(JointInstanceIndex[i], JointTransforms[i], true);
	}
}

void AVR_Hand_Tracked::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UHeadMountedDisplayFunctionLibrary::GetHandTrackingState(this, EXRSpaceType::UnrealWorldSpace, 
		HandType, TrackedHandData);
	
	if (!TrackedHandData.bValid)
		return;
	
	
	RecordJointTransforms(TrackedHandData);
	DrawHand();
}
