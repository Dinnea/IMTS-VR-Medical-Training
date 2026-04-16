#include "GrabbableItem.h"
#include "SpawnZone.h"
#include "VR_Hands/VR_Hand_Tracked.h"

AGrabbableItem::AGrabbableItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Collider = CreateDefaultSubobject<UBoxComponent>("Collider");	
	SetRootComponent(Collider);
	Collider->SetSimulatePhysics(true);
	Collider->SetEnableGravity(true);
}

void AGrabbableItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	FVector MeshOrigin = FVector(0.0f, 0.0f, 0.0f);
	FVector BoxSize = FVector(0.0f, 0.0f, 0.0f);
	
	if (const auto* StaticMeshComponent = Cast<UStaticMeshComponent>(MeshComp))
		StaticMeshComponent->GetLocalBounds(MeshOrigin, BoxSize);
	
	else if (const auto* SkinnedMeshComponent = Cast<USkinnedMeshComponent>(MeshComp))
	{

		if (const auto* SkinnedAsset = SkinnedMeshComponent->GetSkinnedAsset())
		{
			const FBoxSphereBounds Bounds = SkinnedAsset->GetBounds();
			BoxSize = Bounds.BoxExtent;
		}
	}
	
	Collider->SetBoxExtent(BoxSize);
}

void AGrabbableItem::Grab(AVR_Hand_Tracked* Hand, const FName SocketName)
{
	if (IsHeld)
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("Grabbing..."));
	IsHeld = true;
	Collider->SetSimulatePhysics(false);
	this->AttachToComponent(Hand->GetHandMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void AGrabbableItem::Drop()
{
	UE_LOG(LogTemp, Warning, TEXT("Dropping..."));
	
	IsHeld = false;
	//Origin->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	this->DetachRootComponentFromParent(true);
	Collider->SetSimulatePhysics(true);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AGrabbableItem::OnSpawn(ASpawnZone* InSpawnZone)
{
	SpawnZone = InSpawnZone;
}

void AGrabbableItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetAttachParentActor())
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("OVERLAP %s"), *OtherComp->GetName());
	
	if (auto* Spawn = Cast<ASpawnZone> (OtherActor))
		IsInSpawn = Spawn == SpawnZone;
}

void AGrabbableItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetAttachParentActor())
		return;
	
	if (Cast<ASpawnZone> (OtherActor))
		IsInSpawn = false;
}

void AGrabbableItem::BeginPlay()
{
	Super::BeginPlay();
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AGrabbableItem::OnOverlapBegin);
	Collider->OnComponentEndOverlap.AddDynamic(this, &AGrabbableItem::OnOverlapEnd);
}

void AGrabbableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Collider->UpdateOverlaps();
}

