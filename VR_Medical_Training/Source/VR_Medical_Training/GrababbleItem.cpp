#include "GrababbleItem.h"

AGrababbleItem::AGrababbleItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	
	Mesh->SetEnableGravity(true);
}

void AGrababbleItem::Grab(USceneComponent* Source, const FName SocketName)
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbing..."));
	
	Mesh->SetSimulatePhysics(false);
	Mesh->AttachToComponent(Source, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void AGrababbleItem::Drop()
{
	UE_LOG(LogTemp, Warning, TEXT("Dropping..."));
	
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Mesh->SetSimulatePhysics(true);
}

void AGrababbleItem::BeginPlay()
{
	Super::BeginPlay();
}

void AGrababbleItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

