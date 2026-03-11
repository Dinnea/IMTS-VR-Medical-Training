#include "VR_Hand.h"

AVR_Hand::AVR_Hand()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVR_Hand::BeginPlay()
{
	Super::BeginPlay();
}

void AVR_Hand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
