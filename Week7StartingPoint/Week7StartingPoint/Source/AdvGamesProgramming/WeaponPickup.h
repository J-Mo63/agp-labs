// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "WeaponPickup.generated.h"

UENUM(BlueprintType)
enum class WeaponPickupRarity : uint8
{
    LEGENDARY,
    MASTER,
    RARE,
    COMMON
};

enum WeaponStats : uint8
{
    DAMAGE   = 0,
    VELOCITY = 1,
    MAGAZINE = 2,
    ACCURACY = 3,
};

UCLASS()
class ADVGAMESPROGRAMMING_API AWeaponPickup : public APickup
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintImplementableEvent)
    void OnPickup(AActor* ActorThatPickedUp) override;

    UFUNCTION()
    void OnGenerate() override;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    WeaponPickupRarity Rarity;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    float BulletDamage;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    float MuzzleVelocity;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    int32 MagazineSize;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    float WeaponAccuracy;
};
