#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GrabbableItem.generated.h"

class ASpawnZone;
class AVR_Hand_Tracked;

UCLASS(Blueprintable, BlueprintType)
class VR_MEDICAL_TRAINING_API AGrabbableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrabbableItem();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void Grab(AVR_Hand_Tracked* Hand);
	virtual void Drop();
	void OnSpawn(ASpawnZone* InSpawnZone);
	
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	
	UPROPERTY(EditDefaultsOnly)
	FName ObjectName;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<AVR_Hand_Tracked> OwningHand;
	
	bool IsInSpawn;
	
	UFUNCTION()
	void OnComponentHit(  UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundCue> DropSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAudioComponent> DropSFX;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> OutlineMaterial;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> Collider;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMeshComponent> MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool UseMeshOffsetOnGrab = true;
	
	FVector MeshOffset;
	
template <typename TMeshComponent>
	TMeshComponent* CreateMeshComponent(const FName& Name)
	{
		TMeshComponent* NewMesh = CreateDefaultSubobject<TMeshComponent>(Name);
		NewMesh->SetSimulatePhysics(false);
		NewMesh->SetEnableGravity(false);
		NewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NewMesh->SetupAttachment(Collider);
		return NewMesh;
	}
	
	
private:
	UPROPERTY()
	TObjectPtr<ASpawnZone> SpawnZone;
	
	bool IsHeld;
};
