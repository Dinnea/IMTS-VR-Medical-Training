#include "VR_CharacterController.h"

#include "IXRTrackingSystem.h"
#include "VR_Hand.h"
#include "Camera/CameraComponent.h"

AVR_CharacterController::AVR_CharacterController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Origin = CreateDefaultSubobject<USceneComponent>("Origin");
	Origin->SetupAttachment(GetMesh());
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(Origin);
}

void AVR_CharacterController::BeginPlay()
{
	Super::BeginPlay();
	
	SetupVRTrackingOrigin();
	SetupVRHands();
}

void AVR_CharacterController::SetupVRTrackingOrigin()
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

TObjectPtr<AVR_Hand> AVR_CharacterController::SpawnHand(UWorld* World, const TSubclassOf<AVR_Hand> HandClass) const
{
	if (!HandClass)
	{
		UE_LOG(LogTemp, Warning,
			   TEXT("[VR] Failed to spawn Hand from class '%s'. Check spawn collision, abstract class, or missing BP compile."),
			   *HandClass->GetName());
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

void AVR_CharacterController::SetupVRHands()
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

void AVR_CharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVR_CharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

