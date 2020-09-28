// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManager.h"
#include "Pickup.h"
#include "Engine/GameEngine.h"
#include "TimerManager.h"

// Sets default values
APickupManager::APickupManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickupManager::BeginPlay()
{
	Super::BeginPlay();

    GetWorldTimerManager().SetTimer(WeaponSpawnTimer, this, &APickupManager::SpawnWeaponPickup,
                                    FrequencyOfWeaponPickupSpawns, true, 0.0f);
}

void APickupManager::Init(const TArray<FVector>& SpawnLocations, TSubclassOf<class APickup> WeaponPickup, float FrequencyOfSpawn)
{
    PossibleSpawnLocations = SpawnLocations;
    WeaponPickupClass = WeaponPickup;
    FrequencyOfWeaponPickupSpawns = FrequencyOfSpawn;
}

// Called every frame
void APickupManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupManager::SpawnWeaponPickup()
{
    FVector SpawnLocation = PossibleSpawnLocations[FMath::RandRange(0, PossibleSpawnLocations.Num()-1)];
    APickup* WeaponPickup = GetWorld()->SpawnActor<APickup>(
            WeaponPickupClass,SpawnLocation + FVector(0.f, 0.f, 60.f), FRotator::ZeroRotator);
    WeaponPickup->SetLifeSpan(20.0f);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Pickup Spawned"));
    }
}