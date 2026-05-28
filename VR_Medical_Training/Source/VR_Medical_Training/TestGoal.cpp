#include "TestGoal.h"

#include "GrabbableItem.h"

ATestGoal::ATestGoal()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Volume = CreateDefaultSubobject<UStaticMeshComponent>("Volume");
	SetRootComponent(Volume);
}

void ATestGoal::BeginPlay()
{
	Super::BeginPlay();
	
	Volume->OnComponentBeginOverlap.AddDynamic(this, &ATestGoal::OnOverlapBegin);
	Volume->OnComponentEndOverlap.AddDynamic(this, &ATestGoal::OnOverlapEnd);
	
	DisplayMaterial = Volume->CreateDynamicMaterialInstance(0);
	DisplayMaterial->SetVectorParameterValue(TEXT("Colour"), WrongColour);
	
}

void ATestGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestGoal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapBegin"));
	if (OtherActor->IsA(TargetClass))
		
		DisplayMaterial->SetVectorParameterValue(TEXT("Colour"), CorrectColour);
}

void ATestGoal::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapEnd"));
	if (OtherActor->IsA(TargetClass))
		DisplayMaterial->SetVectorParameterValue(TEXT("Colour"), WrongColour);
}

