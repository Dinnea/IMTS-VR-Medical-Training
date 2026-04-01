#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VR_CharacterPawn.generated.h"

class AVR_Hand;
class UCameraComponent;

UCLASS()
class VR_MEDICAL_TRAINING_API AVR_CharacterPawn : public ACharacter
{
	GENERATED_BODY()

public:
	AVR_CharacterPawn();
	
	UFUNCTION(BlueprintCallable)
	void RegisterHandPose(AVR_Hand* Hand);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
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
	void SetupInputContext();
	void SetupVRHands();
	void MoveForward(float Value);
	void MoveRight(float Value);
	
	TObjectPtr<AVR_Hand> SpawnHand(UWorld* World, TSubclassOf<AVR_Hand> HandClass) const;
};
