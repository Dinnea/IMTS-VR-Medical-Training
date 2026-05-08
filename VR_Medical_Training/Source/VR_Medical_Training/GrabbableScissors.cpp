// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabbableScissors.h"

#include "SkeletalMeshAttributes.h"
#include "VR_Hands/VR_Hand_Tracked.h"

void AGrabbableScissors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!OwningHand)
		return;
	FVector Location =	OwningHand->GetHandMesh()->GetBoneLocationByName("XRHand_IndexDistal", EBoneSpaces::WorldSpace);
	this->SetActorLocation(Location);
}
