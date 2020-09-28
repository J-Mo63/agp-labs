// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ProcedurallyGeneratedMap.h"
#include "PickupManager.h"

void AMultiplayerGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessages)
{
    Super::InitGame(MapName, Options, ErrorMessages);

    ProceduralMap = Cast<AProcedurallyGeneratedMap>(UGameplayStatics::GetActorOfClass(
            GetWorld(), AProcedurallyGeneratedMap::StaticClass()));
    PickupManager = GetWorld()->SpawnActor<APickupManager>();
    PickupManager->Init(ProceduralMap->Vertices, WeaponPickupClass, 10.0f);
}