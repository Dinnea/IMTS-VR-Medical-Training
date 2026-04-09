#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrababbleItem.generated.h"

UCLASS()
class VR_MEDICAL_TRAINING_API AGrababbleItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrababbleItem();
	void Grab(USceneComponent* Source, FName SocketName);
	void Drop();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Origin;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

public:	
	virtual void Tick(float DeltaTime) override;
};
