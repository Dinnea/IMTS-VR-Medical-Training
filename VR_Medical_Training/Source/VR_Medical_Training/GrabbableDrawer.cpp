// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabbableDrawer.h"

#include "VR_Hands/VR_Hand_Tracked.h"

void AGrabbableDrawer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!OwningHand)
		return;


	const float XLocation = OwningHand->GetJointTransform(EJoint::Palm).GetLocation().X;
	
	float Distance = ConstraintMin - XLocation;
	
	UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);
	
	Distance = FMath::Clamp(Distance, ConstraintMin, ConstraintMax);
	
	TestLocation = FVector(OriginalLocation.X - Distance, OriginalLocation.Y, OriginalLocation.Z);
	
}

void AGrabbableDrawer::SetMaxOffset(const float MaxOffset)
{
	OriginalLocation = GetActorLocation();
	ConstraintMin = OriginalLocation.X;
	ConstraintMax = ConstraintMin + MaxOffset;
	
	TestLocation = OriginalLocation;
}

void AGrabbableDrawer::Drop()
{
	OwningHand = nullptr;
}

void AGrabbableDrawer::BeginPlay()
{
	Super::BeginPlay();
	
	MeshOffset = FVector::ZeroVector;

}
