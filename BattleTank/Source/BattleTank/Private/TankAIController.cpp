// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"

ATank * ATankAIController::GetControlledTank() const {

	return Cast<ATank>(GetPawn());
}

// Called every frame
void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GetPlayerTank()) {
		// TODO: Move towards the player

		// Aim towards the player
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());

		// TODO: Fire if ready
	}
	
}

void ATankAIController::BeginPlay() {
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	auto PlayerTank = GetPlayerTank();

	// debugging messages :)
	if (!ControlledTank) {
		UE_LOG(LogTemp, Error, TEXT("AIController not possessing a tank"));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("AIController possessing %s"), *(ControlledTank->GetName()));
	}

	if (!PlayerTank) {
		UE_LOG(LogTemp, Error, TEXT("AITank could not find PlayerTank!"));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("AITank found PlayerTank %s"), *(PlayerTank->GetName()));
	}

	UE_LOG(LogTemp, Warning, TEXT("AIController BeginPlay()"));
}

/*
	Find a Player
*/
ATank * ATankAIController::GetPlayerTank() const {
	auto PlayerController = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerController) { return nullptr; }
	return Cast<ATank>(PlayerController);
}