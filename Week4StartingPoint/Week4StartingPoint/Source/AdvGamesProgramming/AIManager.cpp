// Fill out your copyright notice in the Description page of Project Settings.


#include "AIManager.h"
#include "EngineUtils.h"
#include "NavigationNode.h"

// Sets default values
AAIManager::AAIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();
    PopulateNodes();
    CreateAgents();
}

// Called every frame
void AAIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<class ANavigationNode*> AAIManager::GeneratePath(ANavigationNode* StartNode, ANavigationNode* EndNode) {
    return TArray<ANavigationNode*>();
}

void AAIManager::PopulateNodes() {
    for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It) {
        AllNodes.Add(*It);
    }
}

void AAIManager::CreateAgents() {
    for (int I = 0; I < NumAI; I++) {
        ANavigationNode* RandomLocation = AllNodes[FMath::RandRange(0, AllNodes.Num() - 1)];
        GetWorld()->SpawnActor<AEnemyCharacter>(AgentToSpawn,RandomLocation->GetActorLocation(),
                                                RandomLocation->GetActorRotation());
    }
}