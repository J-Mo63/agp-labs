// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MultiplayerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ADVGAMESPROGRAMMING_API AMultiplayerPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

    UFUNCTION(Client, Reliable)
    void ClientDisplayHUD(bool bDisplayHUD);
	
};
