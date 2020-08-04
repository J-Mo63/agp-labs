// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    AutoPossessPlayer = EAutoReceiveInput::Player0;

    LookSensitivity = 75.0f;
    MoveSpeed = 150.0f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

    bUseControllerRotationPitch = true;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &APlayerCharacter::Strafe);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerCharacter::Turn);
    PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
}

void APlayerCharacter::MoveForward(float Value)
{
    FRotator ForwardRotation = GetControlRotation();
    ForwardRotation.Roll = 0.f;
    ForwardRotation.Pitch = 0.f;
    AddMovementInput(ForwardRotation.Vector(), Value * MoveSpeed * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Strafe(float Value)
{
    AddMovementInput(GetActorRightVector(), Value * MoveSpeed * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value * LookSensitivity * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Turn(float Value)
{
    AddControllerYawInput(Value * LookSensitivity * GetWorld()->GetDeltaSeconds());
}