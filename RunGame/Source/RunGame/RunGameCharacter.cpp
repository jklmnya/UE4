// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RunGameCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
// #include "Engine.h"

//////////////////////////////////////////////////////////////////////////
// ARunGameCharacter

ARunGameCharacter::ARunGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	MyARunGameCharacter();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARunGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// PlayerInputComponent->BindAxis("MoveForward", this, &ARunGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARunGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	// PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	// PlayerInputComponent->BindAxis("TurnRate", this, &ARunGameCharacter::TurnAtRate);
	// PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	// PlayerInputComponent->BindAxis("LookUpRate", this, &ARunGameCharacter::LookUpAtRate);

	MySetupPlayerInputComponent(PlayerInputComponent);

}

void ARunGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARunGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARunGameCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

// ------------------------------------------------------------------------------------------ //
void ARunGameCharacter::BeginPlay() {

	Super::BeginPlay();
	// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("KKK"));

}

void ARunGameCharacter::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	if (IsDead) {

	}
	else {
		// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("KKK"));
		GoAhead();
	}
}

void ARunGameCharacter::MyARunGameCharacter() {

	IsDead = false;
	IsLeft = false;
	IsRight = false;
	CanTurn = false;
	DesireRotation = FRotator(0);

}

void ARunGameCharacter::MySetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {

	PlayerInputComponent->BindAction("TurnLeft", IE_Pressed, this, &ARunGameCharacter::SetLeftRotation);
	PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &ARunGameCharacter::SetRightRotation);

}

void ARunGameCharacter::SetRightRotation() {

	if (CanTurn) {
		FRotator RightRotation = FRotator(0, 90.f, 0);
		DesireRotation = CombineRotator(DesireRotation, RightRotation);
	}
	IsRight = true;
	Turn();

}

void ARunGameCharacter::SetLeftRotation() {

	if (CanTurn) {
		FRotator LeftRotation = FRotator(0, -90.f, 0);
		DesireRotation = CombineRotator(DesireRotation, LeftRotation);
	}
	IsLeft = true;
	Turn();
}

void ARunGameCharacter::GoAhead() {

	AddMovementInput(JudgeROrL(DesireRotation), 1.f);

}

// Get MoveFord Vector
FVector ARunGameCharacter::AddInputMoveforward() {

	const FRotator CurrentRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, CurrentRotation.Yaw, 0);
	const FVector X = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	return X;

}

FVector ARunGameCharacter::JudgeROrL(FRotator Rotation) {
	
	if (IsRight) {
		IsRight = false;
	}
	if (IsLeft) {
		IsLeft = false;
	}
	return AddInputMoveforward();

}

FRotator ARunGameCharacter::CombineRotator(FRotator Rotator1, FRotator Rotator2) {

	return FRotator(FQuat(Rotator1) * FQuat(Rotator2));

}

void ARunGameCharacter::Turn() {

	GetController()->SetControlRotation(FMath::RInterpTo(GetControlRotation(), DesireRotation, GetWorld()->GetRealTimeSeconds(), 10.f));

}