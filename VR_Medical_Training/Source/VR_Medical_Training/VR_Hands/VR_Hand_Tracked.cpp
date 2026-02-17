#include "VR_Hand_Tracked.h"
#include "HeadMountedDisplayFunctionLibrary.h"

AVR_Hand_Tracked::AVR_Hand_Tracked()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVR_Hand_Tracked::BeginPlay()
{
	Super::BeginPlay();
	FXRHandTrackingState test;
	UHeadMountedDisplayFunctionLibrary::GetHandTrackingState(this, EXRSpaceType::UnrealWorldSpace, EControllerHand::Left, test);
	
}

void AVR_Hand_Tracked::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

