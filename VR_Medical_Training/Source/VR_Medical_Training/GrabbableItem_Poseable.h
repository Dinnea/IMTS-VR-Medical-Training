#pragma once

#include "CoreMinimal.h"
#include "GrabbableItem.h"
#include "GrabbableItem_Poseable.generated.h"


class UPoseableMeshComponent;

UCLASS()
class VR_MEDICAL_TRAINING_API AGrabbableItem_Poseable : public AGrabbableItem
{
	GENERATED_BODY()
	
public:
	AGrabbableItem_Poseable();
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh;
};
