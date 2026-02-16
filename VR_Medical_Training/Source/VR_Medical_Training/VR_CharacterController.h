#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VR_CharacterController.generated.h"

class UCameraComponent;

UCLASS()
class VR_MEDICAL_TRAINING_API AVR_CharacterController : public ACharacter
{
	GENERATED_BODY()

public:
	AVR_CharacterController();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Origin;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
