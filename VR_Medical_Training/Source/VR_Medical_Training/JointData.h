#pragma once

#include "CoreMinimal.h"
#include "JointData.generated.h"

UENUM(BlueprintType)
enum class EJoint : uint8
{
	Palm,
	Wrist,
	ThumbMetacarpal,
	ThumbProximal,
	ThumbDistal,
	ThumbTip,
	IndexMetacarpal,
	IndexProximal,
	IndexIntermediate,
	IndexDistal,
	IndexTip,
	MiddleMetacarpal,
	MiddleProximal,
	MiddleIntermediate,
	MiddleDistal,
	MiddleTip,
	RingMetacarpal,
	RingProximal,
	RingIntermediate,
	RingDistal,
	RingTip,
	LittleMetacarpal,
	LittleProximal,
	LittleIntermediate,
	LittleDistal,
	LittleTip,
	None = 99
};

USTRUCT(BlueprintType)
struct FJointBoneMap
{
	GENERATED_BODY()

	FJointBoneMap()
	{
		
	}
	explicit FJointBoneMap(FName InBoneName)
	{
		BoneName = InBoneName;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EJoint Joint = EJoint::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName BoneName = "Null";
};
