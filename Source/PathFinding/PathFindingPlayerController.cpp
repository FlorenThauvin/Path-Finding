#include "PathFindingPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "PathFindingCharacter.h"
#include "Engine/World.h"

APathFindingPlayerController::APathFindingPlayerController(){
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void APathFindingPlayerController::PlayerTick(float DeltaTime){
	Super::PlayerTick(DeltaTime);
	if (bMoveToMouseCursor){
		MoveToMouseCursor();
	}
}

void APathFindingPlayerController::SetupInputComponent(){
	Super::SetupInputComponent();
	InputComponent->BindAction("SetDestination", IE_Pressed, this, &APathFindingPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &APathFindingPlayerController::OnSetDestinationReleased);
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &APathFindingPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &APathFindingPlayerController::MoveToTouchLocation);
	InputComponent->BindAction("ResetVR", IE_Pressed, this, &APathFindingPlayerController::OnResetVR);
}

void APathFindingPlayerController::OnResetVR(){
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APathFindingPlayerController::MoveToMouseCursor(){
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled()){
		if (APathFindingCharacter* MyPawn = Cast<APathFindingCharacter>(GetPawn())){
			if (MyPawn->GetCursorToWorld()){
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit){
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void APathFindingPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location){
	FVector2D ScreenSpaceLocation(Location);
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit){
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void APathFindingPlayerController::SetNewMoveDestination(const FVector DestLocation){
	APawn* const MyPawn = GetPawn();
	if (MyPawn){
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());
		if ((Distance > 120.0f)){
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void APathFindingPlayerController::OnSetDestinationPressed(){
	bMoveToMouseCursor = true;
}

void APathFindingPlayerController::OnSetDestinationReleased(){
	bMoveToMouseCursor = false;
}
