#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnZone.generated.h"

class UBoxComponent;
class AGrabbableItem;

UCLASS()
class VR_MEDICAL_TRAINING_API ASpawnZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnZone();
	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> Zone;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGrabbableItem> ToSpawn;
};
