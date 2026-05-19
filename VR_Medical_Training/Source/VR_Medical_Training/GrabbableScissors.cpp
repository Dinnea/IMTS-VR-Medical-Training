// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabbableScissors.h"

#include "SkeletalMeshAttributes.h"
#include "VR_Hands/VR_Hand_Tracked.h"

AGrabbableScissors::AGrabbableScissors()
{
	// Test = CreateDefaultSubobject<USceneComponent>("Test");
	// Test->SetupAttachment(Offset);
	// Collider->SetupAttachment(Test);
}

void AGrabbableScissors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!OwningHand)
		return;
	
	FVector Location =	OwningHand->GetHandMesh()->GetBoneLocationByName("XRHand_IndexDistal", EBoneSpaces::WorldSpace);
	
	this->SetActorLocation(Location+MeshOffset);
	
	double Pitch = 0;
	double Yaw = 0;
	double Roll = 0;

	const FRotator WristRotator = OwningHand->GetJointTransform(EJoint::Wrist).GetRotation().Rotator();
	const FRotator PalmRotator = OwningHand->GetJointTransform(EJoint::Palm).GetRotation().Rotator();
	Pitch = PalmRotator.Pitch + 180;
	//Yaw = PalmRotator.Yaw;
	FRotator TargetRotation = FRotator(Pitch, Yaw, Roll);
	this->SetActorRotation(TargetRotation);
}
