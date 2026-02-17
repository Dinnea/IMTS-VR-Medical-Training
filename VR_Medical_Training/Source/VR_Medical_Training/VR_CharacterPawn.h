#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VR_CharacterPawn.generated.h"

class AVR_Hand_Controller;
class UCameraComponent;

UCLASS()
class VR_MEDICAL_TRAINING_API AVR_CharacterPawn : public ACharacter
{
	GENERATED_BODY()

public:
	AVR_CharacterPawn();
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Origin;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR|Hands")
	TSubclassOf<AVR_Hand_Controller> LeftHandClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR|Hands")
	TSubclassOf<AVR_Hand_Controller> RightHandClass;

private:
	void SetupVRTrackingOrigin();
	void SetupInputContext();
	void SetupVRHands();
	
	TObjectPtr<AVR_Hand_Controller> SpawnHand(UWorld* World, TSubclassOf<AVR_Hand_Controller> HandClass) const;
	
	UPROPERTY(Transient)
	TObjectPtr<AVR_Hand_Controller> LeftHand = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<AVR_Hand_Controller> RightHand = nullptr;
};
