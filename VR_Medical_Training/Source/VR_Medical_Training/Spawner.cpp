// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "GrabbableItem.h"

USpawner::USpawner()
{
}


void USpawner::SpawnObjects()
{
	const FVector SpawnRange = GetScaledBoxExtent();
	
	if (!IsValid(ObjectToSpawn))return;
	
	UWorld* World = GetWorld();
	
	if (!World) return;
	
	for (int i = 0; i < SpawnObjectCount; i++)
	{
		//local point
		FVector RandomPoint = FVector(
			FMath::RandRange(-SpawnRange.X, SpawnRange.X),
			FMath::RandRange(-SpawnRange.Y, SpawnRange.Y),
			FMath::RandRange(-SpawnRange.Z, SpawnRange.Z)
		);
	
		const FVector SpawnLocation = GetComponentTransform().TransformPosition(RandomPoint);
	
		World->SpawnActor<AGrabbableItem>(ObjectToSpawn, SpawnLocation, FRotator::ZeroRotator);
	}
}

void USpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnObjects();
}


void USpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

