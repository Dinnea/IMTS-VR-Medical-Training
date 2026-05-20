// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabbableDrawer.h"

#include "VR_Hands/VR_Hand_Tracked.h"

void AGrabbableDrawer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!OwningHand)
		return;


	const float XLocation = OwningHand->GetJointTransform(EJoint::Palm).GetLocation().X;
	
	float Distance =  FMath::Abs(DrawerOrigin.X - XLocation);
	
	UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);
	
	Distance = FMath::Clamp(Distance, 0, ConstraintMax);
	
	UE_LOG(LogTemp, Warning, TEXT("Distance clamped: %f"), Distance);
	
	FVector Target = FVector(DrawerOrigin.X-Distance, DrawerOrigin.Y, DrawerOrigin.Z);
	
	SetActorLocation(Target);
	
	UE_LOG(LogTemp, Warning, TEXT("Target location: %s"), *Target.ToString());
	
}

void AGrabbableDrawer::SetMaxOffset(const FVector OriginalLocation, const float MaxOffset)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Origiinal location: %s"), *OriginalLocation.ToString());
	DrawerOrigin = OriginalLocation;
	
	ConstraintMin = OriginalLocation.X;
	ConstraintMax = MaxOffset;
	
}

void AGrabbableDrawer::Drop()
{
	OwningHand = nullptr;
}

void AGrabbableDrawer::BeginPlay()
{
	Super::BeginPlay();
}
