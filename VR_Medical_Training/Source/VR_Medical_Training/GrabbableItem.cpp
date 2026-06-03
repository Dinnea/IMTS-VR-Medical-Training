#include "GrabbableItem.h"
#include "SpawnZone.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "VR_Hands/VR_Hand_Tracked.h"

AGrabbableItem::AGrabbableItem()
{
	PrimaryActorTick.bCanEverTick = true;	
	
	Collider = CreateDefaultSubobject<UBoxComponent>("Collider");	
	Collider->SetSimulatePhysics(true);
	Collider->SetEnableGravity(true);
	
	SetRootComponent(Collider);	
}

void AGrabbableItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FBoxSphereBounds Bounds;
	if (const auto* SkinnedMeshComponent = Cast<USkinnedMeshComponent>(MeshComp))
	{
		if (const auto* SkinnedAsset = SkinnedMeshComponent->GetSkinnedAsset())
				Bounds = SkinnedAsset->GetBounds();
	}
	else return;
	
	FVector BoxSize = Bounds.BoxExtent * 1.1f;
	Collider->SetBoxExtent(BoxSize);
}

void AGrabbableItem::Grab(AVR_Hand_Tracked* Hand)
{
	if (OwningHand)
		return;
	
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	OwningHand = Hand;
	Collider->SetSimulatePhysics(false);
	if (UseMeshOffsetOnGrab) SetActorRelativeLocation(MeshOffset);
}

void AGrabbableItem::Drop()
{
	OwningHand = nullptr;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Collider->SetSimulatePhysics(true);
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
		if (OutlineMaterial) MeshComp->SetOverlayMaterial(OutlineMaterial);
		
	
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


void AGrabbableItem::PlayImpactSFX(const FHitResult& Hit)
{
	SoundCooldown = SoundCooldownPeriod;

	if (DropSFX)
		UGameplayStatics::PlaySoundAtLocation(this, DropSFX, this->GetActorLocation());
}

void AGrabbableItem::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (SoundCooldown <= 0)
		PlayImpactSFX(Hit);
}

void AGrabbableItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (MeshComp)
	MeshOffset = MeshComp->GetRelativeLocation();
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AGrabbableItem::OnOverlapBegin);
	Collider->OnComponentEndOverlap.AddDynamic(this, &AGrabbableItem::OnOverlapEnd);
	Collider->OnComponentHit.AddDynamic(this, &AGrabbableItem::OnComponentHit);
	
	Collider->SetNotifyRigidBodyCollision(true);
}

void AGrabbableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Collider->UpdateOverlaps();
	
	if (SoundCooldown < 0 )
		SoundCooldown -= DeltaTime;
	
}

