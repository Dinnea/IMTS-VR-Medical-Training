#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrababbleItem.generated.h"

class UBoxComponent;
class ASpawnZone;
class AVR_Hand_Tracked;

UCLASS()
class VR_MEDICAL_TRAINING_API AGrababbleItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrababbleItem();
	virtual void OnConstruction(const FTransform& Transform) override;
	
	void Grab(AVR_Hand_Tracked* Hand, FName SocketName);
	void Drop() const;
	
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(const AActor* OtherActor, const UPrimitiveComponent* OtherComp);
	
	UFUNCTION(BlueprintCallable)
	void OnOverlapEnd(const AActor* OtherActor, const UPrimitiveComponent* OtherComp);
	
	UPROPERTY(BlueprintReadWrite)
	bool OnSpawn;

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
