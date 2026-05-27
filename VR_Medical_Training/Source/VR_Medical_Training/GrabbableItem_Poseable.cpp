#include "GrabbableItem_Poseable.h"
#include "Components/PoseableMeshComponent.h"

AGrabbableItem_Poseable::AGrabbableItem_Poseable()
{
	MeshComp = CreateMeshComponent<USkeletalMeshComponent>("Mesh");
}
