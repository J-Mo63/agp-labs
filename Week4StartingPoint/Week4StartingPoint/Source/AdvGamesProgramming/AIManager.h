// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCharacter.h"
#include "AIManager.generated.h"

UCLASS()
class ADVGAMESPROGRAMMING_API AAIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIManager();

    TArray<class ANavigationNode*> GeneratePath(ANavigationNode* StartNode, ANavigationNode* EndNode);

    UPROPERTY(EditAnywhere, Category = "AI Properties")
    int32 NumAI;

    UPROPERTY(VisibleAnywhere, Category = "Navigation Nodes")
    TArray<ANavigationNode*> AllNodes;

    UPROPERTY(VisibleAnywhere, Category = "Agents")
    TArray<AEnemyCharacter*> AllAgents;

    UPROPERTY(EditAnywhere, Category = "Agents")
    TSubclassOf<AEnemyCharacter> AgentToSpawn;

    void PopulateNodes();

    void CreateAgents();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
