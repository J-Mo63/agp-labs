// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

APlayerHUD::APlayerHUD()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> PlayerHUDObject(TEXT("/Game/Widgets/PlayerHUDWidget"));
    PlayerHUDClass = PlayerHUDObject.Class;

    if (PlayerHUDClass)
    {
        CurrentPlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
        if (CurrentPlayerHUDWidget)
        {
            CurrentPlayerHUDWidget->AddToViewport();
            HealthProgressBar = Cast<UProgressBar>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("ProgHealthBar")));
            TextBlock = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("RoundsRemainingText")));
        }
    }
}

void APlayerHUD::SetPlayerHealthBarPercent(float Percent)
{
    if (!HealthProgressBar) { return; }
    HealthProgressBar->SetPercent(Percent);
}

void APlayerHUD::SetPlayerRounds(int32 Remaining, int32 Total)
{
    if (!TextBlock) { return; }
    TextBlock->SetText(FText::AsCultureInvariant(
            FString::FromInt(Remaining) + FString(TEXT("/") + FString::FromInt(Total))));
}