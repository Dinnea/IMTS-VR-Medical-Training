#include "SpawnZone.h"

#include "GrabbableItem.h"

ASpawnZone::ASpawnZone()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Zone = CreateDefaultSubobject<UBoxComponent>("Zone");
	RootComponent = Zone;
}

void ASpawnZone::BeginPlay()
{
	Super::BeginPlay();

	AGrabbableItem* Spawned = GetWorld()->SpawnActor<AGrabbableItem>(ToSpawn, GetActorLocation(), GetActorRotation());
	
	Spawned->OnSpawn(this);
}

void ASpawnZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
