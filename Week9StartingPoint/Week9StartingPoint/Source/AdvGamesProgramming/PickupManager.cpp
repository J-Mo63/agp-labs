// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManager.h"
#include "Engine/World.h"
#include "Pickup.h"
#include "Engine/GameEngine.h"

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
    APickup* WeaponPickup = GetWorld()->SpawnActor<APickup>(WeaponPickupClass, SpawnLocation, FRotator::ZeroRotator);
    WeaponPickup->SetLifeSpan(20.0f);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Pickup Spawned"));
    }
}