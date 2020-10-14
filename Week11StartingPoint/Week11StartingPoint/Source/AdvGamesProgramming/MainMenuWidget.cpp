// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameInstance.h"

bool UMainMenuWidget::Initialize()
{
    bool bReturnValue = Super::Initialize();
    GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    ButtonHost->OnClicked.AddDynamic(this, &UMainMenuWidget::OnHostButtonPressed);
    ButtonJoin->OnClicked.AddDynamic(this, &UMainMenuWidget::OnJoinButtonPressed);
    ButtonQuit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitButtonPressed);
    return bReturnValue;
}

void UMainMenuWidget::OnHostButtonPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("Host button pressed"))
    if (GameInstance)
    {
        GameInstance->CreateSession(TEXT("Test Session"));
    }
}

void UMainMenuWidget::OnJoinButtonPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("Join button pressed"))
    if (GameInstance)
    {
        GameInstance->JoinRunningSession(TEXT("Test Session"));
    }
}

void UMainMenuWidget::OnQuitButtonPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("Quit button pressed"))
    UKismetSystemLibrary::QuitGame(
            GetWorld(),
            UGameplayStatics::GetPlayerController(GetWorld(), 0),
            EQuitPreference::Quit,
            false);
}