#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrababbleItem.generated.h"

class ASpawnZone;

UCLASS()
class VR_MEDICAL_TRAINING_API AGrababbleItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrababbleItem();
	void Grab(USceneComponent* Source, FName SocketName);
	void Drop();
	
	UPROPERTY(BlueprintReadWrite)
	bool OnSpawn = false;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Origin;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ASpawnZone> SpawnZone;

public:	
	virtual void Tick(float DeltaTime) override;
};
