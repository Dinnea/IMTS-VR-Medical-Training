// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BrushTip.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR_MEDICAL_TRAINING_API UBrushTip : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBrushTip();
	UPROPERTY(EditAnywhere)
	UMaterialInterface* StrokeDecalMaterial;

	UPROPERTY(EditAnywhere)
	float BrushSize = 2.0f;

	UPROPERTY(EditAnywhere)
	float TraceRadius = 1.0f;

	FVector LastTipLocation;
	bool bHadLastTipLocation = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void DrawAtHit(const FHitResult& Hit);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
