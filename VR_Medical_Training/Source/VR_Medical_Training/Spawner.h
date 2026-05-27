// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Spawner.generated.h"


class UBoxComponent;
class AGrabbableItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR_MEDICAL_TRAINING_API USpawner : public UBoxComponent
{
	GENERATED_BODY()

public:	
	USpawner();
	void SpawnObjects();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGrabbableItem> ObjectToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int SpawnObjectCount;
		
};
