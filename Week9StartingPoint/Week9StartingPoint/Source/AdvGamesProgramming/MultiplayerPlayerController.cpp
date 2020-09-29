// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerPlayerController.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.h"

void AMultiplayerPlayerController::ClientDisplayHUD_Implementation(bool bDisplayHUD)
{
    Cast<APlayerHUD>(GetHUD())->DisplayHUD(bDisplayHUD);
}