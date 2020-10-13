// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "MainMenuWidget.h"
#include "GameFramework/PlayerController.h"

UMainGameInstance::UMainGameInstance(const FObjectInitializer& ObjectInitialize)
{
    static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetObject(TEXT("/Game/Widgets/MainMenuWidget"));
    MainMenuWidgetClass = MainMenuWidgetObject.Class;
}

void UMainGameInstance::LoadMenu()
{
    if (MainMenuWidgetClass)
    {
        Menu = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
        if (Menu)
        {
            Menu->AddToViewport();
        }
    }

    FInputModeUIOnly InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetWidgetToFocus(Menu->TakeWidget());
    GetFirstLocalPlayerController()->bShowMouseCursor = true;
    GetFirstLocalPlayerController()->SetInputMode(InputMode);
}