// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ADVGAMESPROGRAMMING_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

    UMainGameInstance(const FObjectInitializer& ObjectInitialize);

    virtual void Init();

	UFUNCTION(BlueprintCallable)
    void LoadMenu();

    void CreateSession(FName SessionName);

    void JoinRunningSession(FName SessionName);

private:

    void OnCreateSessionComplete(FName SessionName, bool bSuccess);

    void OnStartSessionComplete(FName SessionName, bool bSuccess);

    void OnDestroySessionComplete(FName SessionName, bool bSuccess);
	
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    void OnFindSessionsComplete(bool bWasSuccessful);

    TSubclassOf<class UUserWidget> MainMenuWidgetClass;

    class UMainMenuWidget* Menu;

    IOnlineSubsystem* Subsystem;

    IOnlineSessionPtr SessionInterface;

    TSharedPtr<class FOnlineSessionSearch> SessionSearch;
    TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	
};
