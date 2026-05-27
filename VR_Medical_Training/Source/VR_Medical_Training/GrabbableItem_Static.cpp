#include "GrabbableItem_Static.h"

AGrabbableItem_Static::AGrabbableItem_Static()
{
	MeshComp = CreateMeshComponent<UStaticMeshComponent>("Mesh");
	
	//MeshComp = Mesh;
}
