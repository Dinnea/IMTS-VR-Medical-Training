#include "VR_CharacterController.h"
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
}

void AVR_CharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVR_CharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

