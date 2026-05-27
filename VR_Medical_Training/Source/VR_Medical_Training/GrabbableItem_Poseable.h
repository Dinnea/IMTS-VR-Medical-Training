#pragma once

#include "CoreMinimal.h"
#include "GrabbableItem.h"
#include "GrabbableItem_Poseable.generated.h"


class UPoseableMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class VR_MEDICAL_TRAINING_API AGrabbableItem_Poseable : public AGrabbableItem
{
	GENERATED_BODY()
	
public:
	AGrabbableItem_Poseable();
};
