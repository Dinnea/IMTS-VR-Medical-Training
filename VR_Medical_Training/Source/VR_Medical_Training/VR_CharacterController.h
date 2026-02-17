#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VR_CharacterController.generated.h"

class AVR_Hand;
class UCameraComponent;

UCLASS()
class VR_MEDICAL_TRAINING_API AVR_CharacterController : public ACharacter
{
	GENERATED_BODY()

public:
	AVR_CharacterController();
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Origin;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR|Hands")
	TSubclassOf<AVR_Hand> LeftHandClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR|Hands")
	TSubclassOf<AVR_Hand> RightHandClass;

private:
	void SetupVRTrackingOrigin();
	void SetupInputContext();
	void SetupVRHands();
	
	TObjectPtr<AVR_Hand> SpawnHand(UWorld* World, TSubclassOf<AVR_Hand> HandClass) const;
	
	UPROPERTY(Transient)
	TObjectPtr<AVR_Hand> LeftHand = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<AVR_Hand> RightHand = nullptr;
};
