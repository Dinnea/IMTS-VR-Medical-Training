#include "GrababbleItem.h"

#include "SpawnZone.h"
#include "Components/BoxComponent.h"
#include "VR_Hands/VR_Hand_Tracked.h"

AGrababbleItem::AGrababbleItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetEnableGravity(true);
	RootComponent = Mesh;
}

void AGrababbleItem::Grab(AVR_Hand_Tracked* Hand, const FName SocketName)
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbing..."));
	
	Mesh->SetSimulatePhysics(false);
	this->AttachToComponent(Hand->GetHandMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void AGrababbleItem::Drop() const
{
	UE_LOG(LogTemp, Warning, TEXT("Dropping..."));
	
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Mesh->SetSimulatePhysics(true);
}

void AGrababbleItem::OnOverlapBegin(const AActor* OtherActor, const UPrimitiveComponent* OtherComp)
{
	//UE_LOG(LogTemp, Warning, TEXT("OVERLAP unchecked %s"), *OtherActor->GetName());
	if (!OtherActor || OtherActor == this || OtherActor == GetAttachParentActor())
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("OVERLAP %s"), *OtherComp->GetName());
	
	if (auto* Spawn = Cast<ASpawnZone> (OtherActor))
		OnSpawn = true;
		//OnSpawn = Spawn == SpawnZone;
}

void AGrababbleItem::OnOverlapEnd(const AActor* OtherActor, const UPrimitiveComponent* OtherComp)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetAttachParentActor())
		return;
	
	if (auto* Spawn = Cast<ASpawnZone> (OtherActor))
		OnSpawn = false;
}

void AGrababbleItem::BeginPlay()
{
	Super::BeginPlay();
}

void AGrababbleItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AGrababbleItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Mesh->UpdateOverlaps();
}

