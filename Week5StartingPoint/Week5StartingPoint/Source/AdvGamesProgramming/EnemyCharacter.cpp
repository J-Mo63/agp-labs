// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EngineUtils.h"
#include "HealthComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CurrentAgentState = AgentState::PATROL;
    DetectedActor = nullptr;
    bCanSeeActor = false;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

    HealthComponent = FindComponentByClass<UHealthComponent>();
    if (!HealthComponent) {UE_LOG(LogTemp, Error, TEXT("NO HEALTH COMPONENT FOUND")) };

    PerceptionComponent = FindComponentByClass<UAIPerceptionComponent>();
    if (!PerceptionComponent) {UE_LOG(LogTemp, Error, TEXT("NO PERCEPTION COMPONENT FOUND")) };
    PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::SensePlayer);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HealthComponent) { return; }

    switch (CurrentAgentState)
    {
        case AgentState::PATROL:
            if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() >= .4f)
            {
                CurrentAgentState = AgentState::ENGAGE;
                Path.Empty();
            }
            else if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() < .4f)
            {
                CurrentAgentState = AgentState::EVADE;
                Path.Empty();
            }
            else
            {
                AgentPatrol();
            }
            break;
        case AgentState::ENGAGE:
            if (!bCanSeeActor)
            {
                CurrentAgentState = AgentState::PATROL;
            }
            else if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() < .4f)
            {
                CurrentAgentState = AgentState::EVADE;
                Path.Empty();
            }
            else
            {
                AgentEngage();
            }
            break;
        case AgentState::EVADE:
            if (!bCanSeeActor)
            {
                CurrentAgentState = AgentState::PATROL;
            }
            else if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() >= .4f)
            {
                CurrentAgentState = AgentState::ENGAGE;
                Path.Empty();
            }
            else
            {
                AgentEvade();
            }
            break;
    }
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::AgentPatrol()
{
    if (Path.Num() == 0 && Manager)
    {
        Path = Manager->GeneratePath(CurrentNode, Manager->AllNodes[FMath::RandRange(0, Manager->AllNodes.Num()-1)]);
    }
    else
    {
        MoveAlongPath();
    }
}

void AEnemyCharacter::AgentEngage()
{
    if (bCanSeeActor && DetectedActor)
    {
        FVector DirectionToTarget = DetectedActor->GetActorLocation() - GetActorLocation();
        Fire(DirectionToTarget);
    }

    if (Path.Num() == 0 && Manager && DetectedActor)
    {
        Path = Manager->GeneratePath(CurrentNode,
                                     Manager->FindNearestNode(DetectedActor->GetActorLocation()));
    }
    else
    {
        MoveAlongPath();
    }
}

void AEnemyCharacter::AgentEvade()
{
    if (bCanSeeActor && DetectedActor)
    {
        FVector DirectionToTarget = DetectedActor->GetActorLocation() - GetActorLocation();
        Fire(DirectionToTarget);
    }

    if (Path.Num() == 0 && Manager && DetectedActor)
    {
        Path = Manager->GeneratePath(CurrentNode,
                     Manager->FindFurthestNode(DetectedActor->GetActorLocation()));
    }
    else
    {
        MoveAlongPath();
    }
}

void AEnemyCharacter::SensePlayer(AActor* ActorSensed, FAIStimulus Stimulus)
{
    if (Stimulus.WasSuccessfullySensed())
    {
        UE_LOG(LogTemp, Warning, TEXT("Player Detected"))
        DetectedActor = ActorSensed;
        bCanSeeActor = true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Player Lost"))
        bCanSeeActor = false;
    }
}

void AEnemyCharacter::MoveAlongPath()
{
    if (Path.Num() == 0 || !Manager) { return; }

    //UE_LOG(LogTemp, Display, TEXT("Current Node: %s"), *CurrentNode->GetName())
    if ((GetActorLocation() - CurrentNode->GetActorLocation()).IsNearlyZero(100.0f))
    {
        UE_LOG(LogTemp, Display, TEXT("At Node %s"), *CurrentNode->GetName())
        CurrentNode = Path.Pop();
    }
    else
    {
        FVector WorldDirection = CurrentNode->GetActorLocation() - GetActorLocation();
        WorldDirection.Normalize();
        //UE_LOG(LogTemp, Display, TEXT("The World Direction(X:%f,Y:%f,Z:%f)"), WorldDirection.X, WorldDirection.Y, WorldDirection.Z)
        AddMovementInput(WorldDirection, 1.0f);

        //Get the AI to face in the direction of travel.
        FRotator FaceDirection = WorldDirection.ToOrientationRotator();
        FaceDirection.Roll = 0.f;
        FaceDirection.Pitch = 0.f;
        //FaceDirection.Yaw -= 90.0f;
        SetActorRotation(FaceDirection);
    }
}