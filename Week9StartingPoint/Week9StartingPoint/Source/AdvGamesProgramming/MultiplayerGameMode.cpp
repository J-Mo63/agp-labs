// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ProcedurallyGeneratedMap.h"
#include "PickupManager.h"
#include "Engine/GameEngine.h"
#include "MultiplayerPlayerController.h"
#include "TimerManager.h"

void AMultiplayerGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessages)
{
    Super::InitGame(MapName, Options, ErrorMessages);

    ProceduralMap = Cast<AProcedurallyGeneratedMap>(UGameplayStatics::GetActorOfClass(
            GetWorld(), AProcedurallyGeneratedMap::StaticClass()));
    PickupManager = GetWorld()->SpawnActor<APickupManager>();
    PickupManager->Init(ProceduralMap->Vertices, WeaponPickupClass, 10.0f);
}

void AMultiplayerGameMode::Respawn(AMultiplayerPlayerController* Controller)
{
    if (Controller && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Respawning"));
        APawn* DeadActor = Controller->GetPawn();
        DeadActor->DetachFromControllerPendingDestroy();
        DeadActor->Destroy();
        Controller->ClientDisplayHUD(false);
        RespawnQueue.Add(Controller);
        TimerQueue.Add(FTimerHandle());
        GetWorldTimerManager().SetTimer(TimerQueue.Top(), this,
                                        &AMultiplayerGameMode::ExecuteRespawn, 5.f, false);
    }
}

void AMultiplayerGameMode::ExecuteRespawn()
{
    TimerQueue.RemoveAt(0);
    AMultiplayerPlayerController* Controller = RespawnQueue[0];
    RespawnQueue.RemoveAt(0);
    if (!Controller) { return; }

    AActor* PlayerStart = ChoosePlayerStart(Controller);
    if (PlayerStart)
    {
        APawn* SpawnedActor = GetWorld()->SpawnActor<APawn>(
                DefaultPawnClass, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());
        if (SpawnedActor)
        {
            Controller->Possess(SpawnedActor);
            Controller->ClientDisplayHUD(true);
        }
    }
}