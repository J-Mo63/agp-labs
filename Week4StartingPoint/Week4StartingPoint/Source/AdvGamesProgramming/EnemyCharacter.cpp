// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "AIManager.h"
#include "NavigationNode.h"
#include "Engine/World.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Path.Num() > 0)
	{
	    if ((GetActorLocation() - CurrentNode->GetActorLocation()).IsNearlyZero(100.0f))
        {
	        CurrentNode = Path.Pop();
        }
	    else {
	        FVector Heading = CurrentNode->GetActorLocation() - GetActorLocation();
            AddMovementInput(Heading);
            float LerpedDirection = FMath::Lerp(GetActorForwardVector().Rotation().Yaw,
                                                  Heading.Rotation().Yaw,
                                            GetWorld()->GetDeltaSeconds() * 10.f);
            SetActorRotation(FRotator(0, LerpedDirection, 0));
	    }
	}
	else
    {
        Path = Manager->GeneratePath(
                CurrentNode,Manager->AllNodes[FMath::RandRange(0, Manager->AllNodes.Num() - 1)]);
    }
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

