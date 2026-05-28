#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestGoal.generated.h"

class AGrabbableItem;
class UBoxComponent;

UCLASS()
class VR_MEDICAL_TRAINING_API ATestGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	ATestGoal();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Volume;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGrabbableItem> TargetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor CorrectColour = FLinearColor::Green;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor WrongColour = FLinearColor::Red;
	
private:
	UPROPERTY();
	TObjectPtr<UMaterialInstanceDynamic> DisplayMaterial;

};
