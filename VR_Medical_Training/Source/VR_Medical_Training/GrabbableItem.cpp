#include "GrabbableItem.h"
#include "SpawnZone.h"
#include "Components/AudioComponent.h"
#include "VR_Hands/VR_Hand_Tracked.h"

AGrabbableItem::AGrabbableItem()
{
	PrimaryActorTick.bCanEverTick = true;	
	
	Collider = CreateDefaultSubobject<UBoxComponent>("Collider");	
	Collider->SetSimulatePhysics(true);
	Collider->SetEnableGravity(true);
	
	SetRootComponent(Collider);	
	
	DropSFX = CreateDefaultSubobject<UAudioComponent>("DropSFX");
	
	//HoverHighlight->SetupAttachment(Origin);
	DropSFX->SetupAttachment(Collider);
}

void AGrabbableItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FBoxSphereBounds Bounds;
	// if (const auto* StaticMeshComponent = Cast<UStaticMeshComponent>(MeshComp))
	// {
	// 	if (const auto StaticMesh = StaticMeshComponent->GetStaticMesh())
	// 		Bounds = StaticMesh->GetBounds();
	// }
	//
	// else 
	if (const auto* SkinnedMeshComponent = Cast<USkinnedMeshComponent>(MeshComp))
	{
		if (const auto* SkinnedAsset = SkinnedMeshComponent->GetSkinnedAsset())
				Bounds = SkinnedAsset->GetBounds();
	}
	else return;
	
	UE_LOG(LogTemp, Warning, TEXT("pinch hold %s"), *Bounds.BoxExtent.ToString());
	FVector BoxSize = Bounds.BoxExtent * 1.1f;
	Collider->SetBoxExtent(BoxSize);
}

void AGrabbableItem::Grab(AVR_Hand_Tracked* Hand)
{
	if (OwningHand)
		return;
	
	OwningHand = Hand;
	Collider->SetSimulatePhysics(false);
	if (UseMeshOffsetOnGrab) SetActorRelativeLocation(MeshOffset);
}

void AGrabbableItem::Drop()
{
	OwningHand = nullptr;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Collider->SetSimulatePhysics(true);
	//Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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
	
	if (Cast<AVR_Hand>(OtherActor))
		
	{ if (!OutlineMaterial) UE_LOG(LogTemp, Warning, TEXT("OutlineMaterial missing")); }
		//MeshComp->SetOverlayMaterial(OutlineMaterial); }
		
	
	if (auto* Spawn = Cast<ASpawnZone> (OtherActor))
		IsInSpawn = Spawn == SpawnZone;
}

void AGrabbableItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetAttachParentActor())
		return;
	
	if (Cast<AVR_Hand>(OtherActor))
		MeshComp->SetOverlayMaterial(nullptr);
	
	if (Cast<ASpawnZone> (OtherActor))
		IsInSpawn = false;
}


void AGrabbableItem::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DropSFX ->Play();
}

void AGrabbableItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (MeshComp)
	MeshOffset = MeshComp->GetRelativeLocation();
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AGrabbableItem::OnOverlapBegin);
	Collider->OnComponentEndOverlap.AddDynamic(this, &AGrabbableItem::OnOverlapEnd);
	Collider->OnComponentHit.AddDynamic(this, &AGrabbableItem::OnComponentHit);
}

void AGrabbableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Collider->UpdateOverlaps();
}

