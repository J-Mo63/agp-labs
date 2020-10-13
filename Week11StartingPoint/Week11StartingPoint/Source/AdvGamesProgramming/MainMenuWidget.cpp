// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"

bool UMainMenuWidget::Initialize()
{
    bool bReturnValue = Super::Initialize();
    ButtonHost->OnClicked.AddDynamic(this, &UMainMenuWidget::OnHostButtonPressed);
    ButtonJoin->OnClicked.AddDynamic(this, &UMainMenuWidget::OnJoinButtonPressed);
    ButtonQuit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitButtonPressed);
    return bReturnValue;
}

void UMainMenuWidget::OnHostButtonPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("Host button pressed"))
}

void UMainMenuWidget::OnJoinButtonPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("Join button pressed"))
}

void UMainMenuWidget::OnQuitButtonPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("Quit button pressed"))
}