#include "GrababbleItem.h"
#include "Components/BoxComponent.h"
#include "VR_Hands/VR_Hand_Tracked.h"

AGrababbleItem::AGrababbleItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	
	Collider = CreateDefaultSubobject<UBoxComponent>("Collider");
	Collider->SetupAttachment(Mesh);
	
	Mesh->SetEnableGravity(true);
}

void AGrababbleItem::Grab(AVR_Hand_Tracked* Hand, const FName SocketName)
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbing..."));
	
	Mesh->SetSimulatePhysics(false);
	Mesh->AttachToComponent(Hand->GetHandMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	Mesh->SetCollisionProfileName("Trigger");
	this->UpdateOverlaps();
}

void AGrababbleItem::Drop()
{
	UE_LOG(LogTemp, Warning, TEXT("Dropping..."));
	
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionProfileName("Physics Object");
	this->UpdateOverlaps();
}

void AGrababbleItem::OnOverlapBegin(const AActor* OtherActor) const
{
	UE_LOG(LogTemp, Warning, TEXT("OVERLAP unchecked %s"), *OtherActor->GetName());
	if (!OtherActor || OtherActor == this)
		return;

	if (OtherActor == GetAttachParentActor())
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("OVERLAP %s"), *OtherActor->GetName());
}

void AGrababbleItem::BeginPlay()
{
	Super::BeginPlay();
}

void AGrababbleItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	FVector MeshOrigin = FVector(0.0f, 0.0f, 0.0f);
	FVector BoxSize = FVector(0.0f, 0.0f, 0.0f);
	
	Mesh->GetLocalBounds(MeshOrigin, BoxSize);
	Collider->SetBoxExtent(BoxSize);
}

void AGrababbleItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

