// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class ADVGAMESPROGRAMMING_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:

    APlayerHUD();

    void DisplayHUD(bool bDrawHUD);

	void SetPlayerHealthBarPercent(float Percent);

	UFUNCTION(BlueprintCallable)
    void SetPlayerRounds(int32 Remaining, int32 Total);

private:

    TSubclassOf<class UUserWidget> PlayerHUDClass;

    UUserWidget* CurrentPlayerHUDWidget;

    class UProgressBar* HealthProgressBar;

    class UTextBlock* TextBlock;
	
};
