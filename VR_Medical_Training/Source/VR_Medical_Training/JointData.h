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

static FString EJointToString(const uint8 Joint)
{
	const UEnum* Enum = StaticEnum<EJoint>();
	return  Enum ? Enum->GetNameStringByValue(Joint) : "Invalid";
}

static FString EJointToString(EJoint Joint)
{
	return EJointToString(static_cast<uint8>(Joint));
}

static FName EJointToName(const EJoint Joint)
{
	return FName(EJointToString(Joint));
}

static FName EJointToName(const uint8 Joint)
{
	return FName(EJointToString(Joint));
}

USTRUCT(BlueprintType)
struct FJointBoneMap
{
	GENERATED_BODY()

	FJointBoneMap()
	{}
	
	explicit  FJointBoneMap(const EJoint InJoint) : Joint(InJoint)
	{}
	
	explicit  FJointBoneMap(const EJoint InJoint, const FName InName) : Joint(InJoint), BoneName(InName)
	{}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EJoint Joint = EJoint::None;

	UPROPERTY(EditAnywhere, meta=(GetOptions="GetBonePool"))
	FName BoneName = NAME_None;
};
