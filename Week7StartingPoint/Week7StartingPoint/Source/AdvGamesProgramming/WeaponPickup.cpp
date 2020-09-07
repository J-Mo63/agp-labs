// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"

void AWeaponPickup::OnGenerate()
{
    APickup::OnGenerate();

    int32 MinGood = 0;
    int32 RarityChance = FMath::RandRange(0, 100);
    if (RarityChance <= 5)
    {
        Rarity = WeaponPickupRarity::LEGENDARY;
        MinGood = 4;
    }
    else if (RarityChance <= 15)
    {
        Rarity = WeaponPickupRarity::MASTER;
        MinGood = 3;
    }
    else if (RarityChance <= 30)
    {
        Rarity = WeaponPickupRarity::RARE;
        MinGood = 1;
    }
    else
    {
        Rarity = WeaponPickupRarity::COMMON;
    }

    TArray<int32> GoodStats;
    while (GoodStats.Num() < MinGood)
    {
        int32 Stat = FMath::RandRange(0, 3);
        if (!GoodStats.Contains(Stat))
        {
            GoodStats.Add(Stat);
        }
    }

    for (int I = 0; I < GoodStats.Num(); I++)
    {
        switch (GoodStats[I])
        {
            case DAMAGE:
                BulletDamage = FMath::RandRange(15.f, 30.f);
                break;
            case VELOCITY:
                MuzzleVelocity = FMath::RandRange(10000.f, 20000.f);
                break;
            case MAGAZINE:
                MagazineSize = FMath::RandRange(20, 100);
                break;
            case ACCURACY:
                WeaponAccuracy = FMath::RandRange(100.f, 1000.f);
                break;
        }
    }

    for (int I = 0; I < 4; I++)
    {
        if (!GoodStats.Contains(I))
        {
            switch (I)
            {
                case DAMAGE:
                    BulletDamage = FMath::RandRange(2.f, 14.f);
                    break;
                case VELOCITY:
                    MuzzleVelocity = FMath::RandRange(5000.f, 9999.f);
                    break;
                case MAGAZINE:
                    MagazineSize = FMath::RandRange(1, 20);
                    break;
                case ACCURACY:
                    WeaponAccuracy = FMath::RandRange(20.f, 80.f);
                    break;
                default:
                    UE_LOG(LogTemp, Error, TEXT("Error while generating attributes"))
                    break;
            }
        }
    }
}