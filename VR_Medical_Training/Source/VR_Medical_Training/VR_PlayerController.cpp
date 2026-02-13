// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_PlayerController.h"

// Sets default values
AVR_PlayerController::AVR_PlayerController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVR_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVR_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVR_PlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

