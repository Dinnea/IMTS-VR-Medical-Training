// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabbableDrawer.h"

#include "VR_Hands/VR_Hand_Tracked.h"

void AGrabbableDrawer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!OwningHand)
		return;

	const FTransform ParentTransform = Parent->GetActorTransform();
	
	FVector HandLocation = OwningHand->GetJointTransform(EJoint::Palm).GetLocation();
	// Make relative to parent
	HandLocation = ParentTransform.InverseTransformPosition(HandLocation);
	const float XLocation = HandLocation.X;
	
	float Distance =  XLocation;
	
	
	Distance = FMath::Clamp(Distance, ConstraintMin, ConstraintMax);
	
	//UE_LOG(LogTemp, Warning, TEXT("Distance clamped: %f"), Distance);
	
	FVector Target = FVector(Distance, DrawerOrigin.Y, DrawerOrigin.Z);
	
	GetParentComponent()->SetRelativeLocation(Target);
	
	//UE_LOG(LogTemp, Warning, TEXT("Target location: %s"), *Target.ToString());
	
}

void AGrabbableDrawer::SetMaxOffset(const FVector OriginalLocation, const float MaxOffset)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Original location: %s"), *OriginalLocation.ToString());
	DrawerOrigin = OriginalLocation;
	
	ConstraintMin = OriginalLocation.X;
	ConstraintMax = MaxOffset;
	
}

void AGrabbableDrawer::Drop()
{
	UE_LOG(LogTemp, Warning, TEXT("drop???"));
	OwningHand = nullptr;
}

void AGrabbableDrawer::BeginPlay()
{
	Super::BeginPlay();
}
