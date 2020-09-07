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

UCLASS()
class ADVGAMESPROGRAMMING_API AWeaponPickup : public APickup
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintImplementableEvent)
    void OnPickup(AActor* ActorThatPickedUp) override;

    void OnGenerate() override;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    float BulletDamage;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    float MuzzleVelocity;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    int32 MagazineSize;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    float WeaponAccuracy;
};
