#include "VR_CharacterPawn.h"

#include "IXRTrackingSystem.h"
#include "VR_Hands/VR_Hand.h"
#include "Camera/CameraComponent.h"
#include "VR_Hands/VR_Hand_Tracked.h"

AVR_CharacterPawn::AVR_CharacterPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Origin = CreateDefaultSubobject<USceneComponent>("Origin");
	RootComponent = Origin;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(Origin);
}

void AVR_CharacterPawn::BeginPlay()
{
	Super::BeginPlay();
	
	SetupVRTrackingOrigin();
	SetupVRHands();
}

void AVR_CharacterPawn::SetupVRTrackingOrigin()
{
	if (!GEngine)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[VR] SetupVRTrackingOrigin aborted: GEngine is null. (Called too early, or running in a context without an engine.)"));
		return;
	}

	const TSharedPtr<IXRTrackingSystem> XRSystem = GEngine->XRSystem;
	if (!XRSystem.IsValid())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[VR] SetupVRTrackingOrigin aborted: XRSystem is invalid (GEngine->XRSystem is null). Is OpenXR enabled and running?"));
		return;
	}
	
	XRSystem->SetTrackingOrigin(EHMDTrackingOrigin::LocalFloor);
	
	UE_LOG(LogTemp, Log,
		TEXT("[VR] Tracking origin set to LocalFloor (XRSystem: %s)."),
		*XRSystem->GetSystemName().ToString());
}

TObjectPtr<AVR_Hand> AVR_CharacterPawn::SpawnHand(UWorld* World, const TSubclassOf<AVR_Hand> HandClass) const
{
	if (!HandClass)
	{
		UE_LOG(LogTemp, Warning,
			   TEXT("[VR] Failed to spawn Hand from class. Check spawn collision, abstract class, or missing BP compile."));
		return nullptr;
	}
	TObjectPtr<AVR_Hand> NewHand = World->SpawnActor<AVR_Hand>(HandClass);
	UE_LOG(LogTemp, Log,
				TEXT("[VR] Spawned + attached Hand: '%s' (Class: %s)"),
				*NewHand->GetName(),
				*HandClass->GetName());
	NewHand->AttachToComponent(Origin, FAttachmentTransformRules::KeepRelativeTransform);
	return NewHand;
}

void AVR_CharacterPawn::SetupVRHands()
{	
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[VR] SetupVRHands aborted: GetWorld() returned null. (Called too early? Try BeginPlay/OnPossessed.)"));
		return;
	}
	
	LeftHand = SpawnHand(World, LeftHandClass);
	RightHand = SpawnHand(World, RightHandClass);
	
}

void AVR_CharacterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
