#include "VR_Hand_Controller.h"
#include "MotionControllerComponent.h"
#include "Components/WidgetInteractionComponent.h"

AVR_Hand_Controller::AVR_Hand_Controller()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Create components
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>("MotionController");
	SetRootComponent(MotionController);
	
	WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>("WidgetInteractionComponent");
	WidgetInteractionComponent->SetupAttachment(HandMesh);
}

void AVR_Hand_Controller::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	switch (HandType)
	{
	case EControllerHand::Left:
		MotionController->MotionSource = "Left";
		bIsAnimationMirrored = true;
		break;
	case EControllerHand::Right:
		MotionController->MotionSource = "Right";
		bIsAnimationMirrored = false;
		break;
	default:
		break;
	} 
}

void AVR_Hand_Controller::BeginPlay()
{
	Super::BeginPlay();
	
	if (HandType != EControllerHand::Left && HandType != EControllerHand::Right)
		if (GEngine) 
			GEngine->AddOnScreenDebugMessage(-1, 4.0, FColor::Red, 
			FString::Printf(TEXT("Class %s: wrong hand type."), *GetClass()->GetName()));
	
}
void AVR_Hand_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

