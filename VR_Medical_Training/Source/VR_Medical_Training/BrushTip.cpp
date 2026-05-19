// Fill out your copyright notice in the Description page of Project Settings.


#include "BrushTip.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBrushTip::UBrushTip()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBrushTip::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UBrushTip::DrawAtHit(const FHitResult& Hit)
{
	if (!StrokeDecalMaterial) return;

	FVector Location = Hit.ImpactPoint;
	FVector Normal = Hit.ImpactNormal;

	// Decal projection direction should face into the surface.
	FRotator Rotation = Normal.Rotation();
	Rotation.Pitch += 90.0f;

	FVector DecalSize(BrushSize, BrushSize, BrushSize);

	UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		StrokeDecalMaterial,
		DecalSize,
		Location + Normal * 0.1f,
		Rotation,
		30.0f // lifetime, use 0 for persistent-ish
	);
}


// Called every frame
void UBrushTip::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector TipLocation = GetComponentLocation();

	if (!bHadLastTipLocation)
	{
		LastTipLocation = TipLocation;
		bHadLastTipLocation = true;
		return;
	}

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.bReturnPhysicalMaterial = false;
	Params.bTraceComplex = true; // important for curved/accurate meshes

	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		LastTipLocation,
		TipLocation,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(TraceRadius),
		Params
	);

	if (bHit && Hit.bBlockingHit)
	{
		DrawAtHit(Hit);
	}

	LastTipLocation = TipLocation;
}

