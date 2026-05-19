#include "GrabbableItem_Static.h"

AGrabbableItem_Static::AGrabbableItem_Static()
{
	Mesh = CreateMeshComponent<UStaticMeshComponent>("Mesh");
}
