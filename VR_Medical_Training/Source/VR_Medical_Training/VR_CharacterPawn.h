#pragma once

#include "CoreMinimal.h"
#include "VR_CharacterPawn.generated.h"

class AVR_Hand;
class UCameraComponent;

UCLASS()
class VR_MEDICAL_TRAINING_API AVR_CharacterPawn : public APawn
{
	GENERATED_BODY()

public:
	AVR_CharacterPawn();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Origin;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR|Hands")
	TSubclassOf<AVR_Hand> LeftHandClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR|Hands")
	TSubclassOf<AVR_Hand> RightHandClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR|Hands")
	TObjectPtr<AVR_Hand> LeftHand = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR|Hands")
	TObjectPtr<AVR_Hand> RightHand = nullptr;
	
private:
	void SetupVRTrackingOrigin();
	void SetupVRHands();
	
	TObjectPtr<AVR_Hand> SpawnHand(UWorld* World, TSubclassOf<AVR_Hand> HandClass) const;
};
