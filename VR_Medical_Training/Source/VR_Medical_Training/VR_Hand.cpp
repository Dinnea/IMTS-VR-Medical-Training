#include "VR_Hand.h"
#include "MotionControllerComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetInteractionComponent.h"

AVR_Hand::AVR_Hand()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Create components
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>("MotionController");
	SetRootComponent(MotionController);
	
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HandMesh");
	HandMesh->SetupAttachment(RootComponent);
	
	WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>("WidgetInteractionComponent");
	WidgetInteractionComponent->SetupAttachment(HandMesh);
	
	GrabShpere = CreateDefaultSubobject<USphereComponent>("GrabShpere");
	GrabShpere->SetupAttachment(HandMesh);
}

void AVR_Hand::OnConstruction(const FTransform& Transform)
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

void AVR_Hand::BeginPlay()
{
	Super::BeginPlay();
	
	if (HandType != EControllerHand::Left && HandType != EControllerHand::Right)
		if (GEngine) 
			GEngine->AddOnScreenDebugMessage(-1, 4.0, FColor::Red, 
			FString::Printf(TEXT("Class %s: wrong hand type."), *GetClass()->GetName()));
	
}
void AVR_Hand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

