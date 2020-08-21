// Fill out your copyright notice in the Description page of Project Settings.


#include "AIManager.h"
#include "EngineUtils.h"
#include "NavigationNode.h"
#include "Math/NumericLimits.h"
#include "EnemyCharacter.h"

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
    OpenSet = TArray<ANavigationNode*>();
    PopulateNodes();
    CreateAgents();
}

// Called every frame
void AAIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


TArray<ANavigationNode*> AAIManager::GeneratePath(ANavigationNode* StartNode, ANavigationNode* EndNode)
{
    OpenSet.Add(StartNode);

    for (auto It = AllNodes.CreateConstIterator(); It; ++It)
    {
        (*It)->GScore = TNumericLimits<float>::Max();
    }

    StartNode->GScore = 0;
    StartNode->HScore = FVector::Dist(StartNode->GetActorLocation(), EndNode->GetActorLocation());

    while (OpenSet.Num() > 0)
    {
        Algo::SortBy(OpenSet, &ANavigationNode::FScore);
        ANavigationNode* CurrentNode = OpenSet.Pop();

        if (CurrentNode == EndNode)
        {
            TArray<ANavigationNode*> Path = TArray<ANavigationNode*>();

            while (CurrentNode != StartNode) {
                Path.Push(CurrentNode);
                check(CurrentNode) // Debug check for null node
                CurrentNode = CurrentNode->CameFrom;
            }
            return Path;
        }

        for (auto It = CurrentNode->ConnectedNodes.CreateConstIterator(); It; ++It)
        {
            ANavigationNode* ConnectedNode = *It;
            float TentativeGScore = CurrentNode->GScore + FVector::Dist(CurrentNode->GetActorLocation(),
                                                                        ConnectedNode->GetActorLocation());

            if (TentativeGScore < ConnectedNode->GScore)
            {
                ConnectedNode->CameFrom = CurrentNode;
                ConnectedNode->GScore = TentativeGScore;
                ConnectedNode->HScore = FVector::Dist(ConnectedNode->GetActorLocation(),
                                                      EndNode->GetActorLocation());
                if (!OpenSet.Contains(ConnectedNode))
                {
                    OpenSet.Add(ConnectedNode);
                }
            }
        }
    }
    return TArray<ANavigationNode*>();
}


void AAIManager::PopulateNodes()
{
    for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It)
    {
        AllNodes.Add(*It);
    }
}

void AAIManager::CreateAgents()
{
    for (int I = 0; I < NumAI; I++)
    {
        ANavigationNode* RandomLocation = AllNodes[FMath::RandRange(0, AllNodes.Num() - 1)];
        AEnemyCharacter* Agent = GetWorld()->SpawnActor<AEnemyCharacter>(AgentToSpawn, RandomLocation->GetActorLocation(),
                                                RandomLocation->GetActorRotation());
        Agent->Manager = this;
        Agent->CurrentNode = RandomLocation;
    }
}