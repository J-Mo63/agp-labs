// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "MainMenuWidget.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

UMainGameInstance::UMainGameInstance(const FObjectInitializer& ObjectInitialize)
{
    static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetObject(TEXT("/Game/Widgets/MainMenuWidget"));
    MainMenuWidgetClass = MainMenuWidgetObject.Class;

    OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMainGameInstance::OnCreateSessionComplete);
    OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UMainGameInstance::OnStartSessionComplete);
    OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UMainGameInstance::OnFindSessionsComplete);
    OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMainGameInstance::OnDestroySessionComplete);
    OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UMainGameInstance::OnJoinSessionComplete);
}

void UMainGameInstance::Init()
{
    Super::Init();
    Subsystem = IOnlineSubsystem::Get();

    if (Subsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Found Online Subsystem: %s”"), *Subsystem->GetSubsystemName().ToString())
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Unable to find Online Subsystem"))
    }

    SessionInterface = Subsystem->GetSessionInterface();
//    SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMainGameInstance::OnFindSessionsComplete);
//    SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMainGameInstance::OnCreateSessionComplete);
//    SessionInterface->OnStartSessionCompleteDelegates.AddUObject(this, &UMainGameInstance::OnStartSessionComplete);
//    SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMainGameInstance::OnDestroySessionComplete);
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

void UMainGameInstance::CreateSession(FName SessionName)
{
    if (SessionInterface.IsValid())
    {
        SessionSettings = MakeShareable(new FOnlineSessionSettings());
        SessionSettings->bIsLANMatch = true;
        SessionSettings->bShouldAdvertise = true;
        SessionSettings->NumPublicConnections = 3;
        SessionSettings->NumPrivateConnections = 3;
        SessionSettings->bAllowJoinInProgress = true;
        OnCreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
        SessionInterface->CreateSession(*GetFirstGamePlayer()->GetPreferredUniqueNetId(), SessionName, *SessionSettings);
    }
}

void UMainGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
    SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
    if (SessionInterface.IsValid() && bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("Session Created Successfully"))
        OnStartSessionCompleteDelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
        SessionInterface->StartSession(SessionName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Session is already in use, restarting session"))
        OnDestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
        SessionInterface->DestroySession(SessionName);
    }
}

void UMainGameInstance::OnStartSessionComplete(FName SessionName, bool bSuccess)
{
    if (bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("Session Started Successfully"))
        SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
        FInputModeGameOnly InputMode;
        InputMode.SetConsumeCaptureMouseDown(true);
        GetFirstLocalPlayerController()->bShowMouseCursor = false;
        GetFirstLocalPlayerController()->SetInputMode(InputMode);

        UGameplayStatics::OpenLevel(GetWorld(), TEXT("MultiplayerMap"), true, "listen");
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Could not start session"))
    }
}

void UMainGameInstance::JoinRunningSession(FName SessionName)
{
    if (SessionInterface.IsValid())
    {
        SessionSearch = MakeShareable(new FOnlineSessionSearch());
        SessionSearch->bIsLanQuery = true;
        SessionSearch->MaxSearchResults = 20;
        SessionSearch->PingBucketSize = 50;
        TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();
        OnFindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
        SessionInterface->FindSessions(*GetFirstGamePlayer()->GetPreferredUniqueNetId(), SearchSettingsRef);
    }
}

void UMainGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
    SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
    if (SessionInterface.IsValid() && SessionSearch.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("%i Sessions Found"), SessionSearch->SearchResults.Num())
        if (SessionSearch->SearchResults.Num() > 0)
        {
            OnJoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
            FOnlineSessionSearchResult SessionResult = SessionSearch->SearchResults[0];
            SessionInterface->JoinSession(*GetFirstGamePlayer()->GetPreferredUniqueNetId(),
                                          *SessionResult.Session.GetSessionIdStr(), SessionResult);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No Sessions could be found"))
    }
}

void UMainGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (SessionInterface.IsValid() && Result == EOnJoinSessionCompleteResult::Success)
    {
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

        UE_LOG(LogTemp, Warning, TEXT("Session joined successfully"))
        FString TravelURL;
        if (SessionInterface->GetResolvedConnectString(SessionName, TravelURL))
        {
            UE_LOG(LogTemp, Warning, TEXT("Client travelling"))
            GetFirstLocalPlayerController()->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
        }
    }
}

void UMainGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
    if (bSuccess)
    {
        SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
        CreateSession(SessionName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Session could not be created"))
        SessionInterface->DestroySession(SessionName);
    }
}