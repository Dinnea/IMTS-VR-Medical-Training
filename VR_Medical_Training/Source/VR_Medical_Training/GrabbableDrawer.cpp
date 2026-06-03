// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabbableDrawer.h"

#include "Spawner.h"
#include "SpawnZone.h"
#include "VR_Hands/VR_Hand_Tracked.h"

AGrabbableDrawer::AGrabbableDrawer()
{
	Spawner = CreateDefaultSubobject<USpawner>("SpawnPoint");
	Spawner ->SetupAttachment(RootComponent);
}

void AGrabbableDrawer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!OwningHand)
		return;

	const FTransform ParentTransform = Parent->GetActorTransform();
	
	FVector HandLocation = OwningHand->GetJointTransform(EJoint::Palm).GetLocation();
	// Make relative to parent
	HandLocation = ParentTransform.InverseTransformPosition(HandLocation);

	const float Distance = FMath::Clamp(HandLocation.X, ConstraintMin, ConstraintMax);
	const FVector Target = FVector(Distance, DrawerOrigin.Y, DrawerOrigin.Z);
	
	GetParentComponent()->SetRelativeLocation(Target);
}

void AGrabbableDrawer::SetMaxOffset(const FVector OriginalLocation, const float MaxOffset)
{
	
	DrawerOrigin = OriginalLocation;
	ConstraintMin = OriginalLocation.X;
	ConstraintMax = MaxOffset;
	
}

void AGrabbableDrawer::Drop()
{
	OwningHand = nullptr;
}

void AGrabbableDrawer::SpawnObjects()
{
	
}

void AGrabbableDrawer::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnObjects();
}
