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
		UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"), *HitLocation.ToString());
		// TODO: Tell controlled tank to aim at this point
		
		// Get world location of linetrace through crosshair

		// if it hits the landscape
		// tell the controller tank to aim at this point
	}
}

// Get world location of linetrace through crosshair, returns if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const {
	// Find the crosshair position
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);
	UE_LOG(LogTemp, Warning, TEXT("ScreenLocation: %s"), *ScreenLocation.ToString());
		// De-project the screen position of the crosshair to a world direction
		// linetrace along that look direction, and see what we hit (up to max range)

	DrawDebugLine(
		GetWorld(),
		GetBarrelLineTraceStart(),
		GetBarrelLineTraceEnd(),
		FColor(255, 0, 0),
		false,
		0.0f,
		0,
		2.0f
	);

	OutHitLocation = GetBarrelLineTraceEnd();

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

//DrawDebugLine(
//	GetWorld(),
//	PlayerViewPointLocation,
//	LineTraceEnd,
//	FColor(255, 0, 0),
//	false,
//	0.0f,
//	0,
//	2.0f
//	);
//
//GetWorld()->LineTraceSingleByObjectType(
//	OUT HitResult,
//	GetReachLineStart(),
//	GetReachLineEnd(),
//	FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
//	TraceParameters
//);
//
///// Setup query parameters
//FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
//
/////  Line-trace (AKA Ray-casting) out to reach distance
//FHitResult HitResult;
//GetWorld()->LineTraceSingleByObjectType(
//	OUT HitResult,
//	GetReachLineStart(),
//	GetReachLineEnd(),
//	FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
//	TraceParameters
//);