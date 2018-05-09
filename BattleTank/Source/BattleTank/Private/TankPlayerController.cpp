// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Error, TEXT("PlayerController not possessing a tank"));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("PlayerController possessing %s"), *(ControlledTank->GetName()));
	}

	UE_LOG(LogTemp, Warning, TEXT("PlayerController BeginPlay()"));
}

// Called every frame
void ATankPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

ATank * ATankPlayerController::GetControlledTank() const {

	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair() {
	if (!GetControlledTank()) { return; }

	FVector HitLocation; // Out parameter
	if (GetSightRayHitLocation(HitLocation)) { 
		GetControlledTank()->AimAt(HitLocation);

		// TODO: Tell controlled tank to aim at this point
		
		// Get world location of linetrace through crosshair

		// if it hits the landscape
		// tell the controller tank to aim at this point
	}
}

// Get world location of linetrace through crosshair, returns if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const {
	// Find the crosshair position in pixel co-ordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	
	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection)) {
		// Linetrace along that look direction, and see what we hit (up to max range)
		// GetLookVectorHitLocation(...);
		GetLookVectorHitLocation(LookDirection, OutHitLocation);
		return true;
	}
	//UE_LOG(LogTemp, Warning, TEXT("ScreenLocation: %s"), *ScreenLocation.ToString());
		
	//OutHitLocation = GetBarrelLineTraceEnd();

	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const {
	FVector WorldLocation;
	
	// De-project the screen position of the crosshair to a world direction
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X, 
		ScreenLocation.Y, 
		WorldLocation, 
		LookDirection
	);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& OutHit) const {
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto StopLocation = StartLocation + (LookDirection * LineTraceRange);

	// if line trace succeeds ... then set hit location, return true
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		StopLocation,
		ECollisionChannel::ECC_Visibility)
	) {
		OutHit = HitResult.Location;

		DrawDebugLine(
			GetWorld(),
			StartLocation,
			StopLocation,
			FColor(255, 0, 0),
			false,
			0.0f,
			0,
			5.0f
		);

		return true;
	}
	
	return false;
}

FVector ATankPlayerController::GetBarrelLineTraceStart() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	//return GetControlledTank()->GetActorLocation();
	return PlayerViewPointLocation;
}


// Get end of line trace, needs starting location and rotation
FVector ATankPlayerController::GetBarrelLineTraceEnd() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * 600;
	/*DrawDebugLine(
	GetWorld(),
	PlayerViewPointLocation,
	LineTraceEnd,
	FColor(255, 0, 0),
	false,
	0.0f,
	0,
	2.0f
	);*/
}
