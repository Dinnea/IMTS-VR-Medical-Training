#include "VR_Hand_Tracked.h"
#include "HeadMountedDisplayFunctionLibrary.h"

AVR_Hand_Tracked::AVR_Hand_Tracked()
{
	PrimaryActorTick.bCanEverTick = true;
	if (GEngine) 
		GEngine->AddOnScreenDebugMessage(-1, 4.0, FColor::Red, 
		FString::Printf(TEXT("Tracked hand built")));
}

void AVR_Hand_Tracked::BeginPlay()
{
	Super::BeginPlay();	
	if (GEngine) 
		GEngine->AddOnScreenDebugMessage(-1, 4.0, FColor::Red, 
		FString::Printf(TEXT("Tracked hand begin play")));
	
}

void AVR_Hand_Tracked::RecordJointTransforms(const FXRHandTrackingState& Data)
{	
	if (GEngine) 
		GEngine->AddOnScreenDebugMessage(-1, 4.0, FColor::Red, 
		FString::Printf(TEXT("Tracked hand record joint transform")));
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
		JointTransforms.Add(
			FTransform(JointsRotations[i],JointsLocations[i], JointScale));
	
}

void AVR_Hand_Tracked::DrawHand()
{
	if (JointMeshes.IsEmpty())
	{
		for (int i = 0; i < 26; i++)
		{
			const FName JointName(TEXT("JointMesh"), i);
			TObjectPtr<UStaticMeshComponent> NewMesh = NewObject<UStaticMeshComponent>(this, JointName);
			NewMesh->SetStaticMesh(JointMesh);
			NewMesh->SetupAttachment(RootComponent);
			
			JointMeshes.Add(NewMesh);
		}
	}
	
	for (int i = 0; i < 26; i++)
	{
		JointMeshes[i]->SetRelativeTransform(JointTransforms[i]);
	}
}

void AVR_Hand_Tracked::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UHeadMountedDisplayFunctionLibrary::GetHandTrackingState(this, EXRSpaceType::UnrealWorldSpace, 
		HandType, TrackedHandData);
	
	if (!TrackedHandData.bValid)
		return;
	if (GEngine) 
		GEngine->AddOnScreenDebugMessage(-1, 4.0, FColor::Red, 
		FString::Printf(TEXT("Tracked hand tick")));
	RecordJointTransforms(TrackedHandData);
	
}
