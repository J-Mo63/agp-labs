// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcedurallyGeneratedMap.h"

// Sets default values
AProcedurallyGeneratedMap::AProcedurallyGeneratedMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh Component"));
}

// Called when the game starts or when spawned
void AProcedurallyGeneratedMap::BeginPlay()
{
	Super::BeginPlay();

    TArray<FVector> Vertices = {
            FVector(0.f, 0.f, 0.f),
            FVector(0.f, 100.f, 0.f),
            FVector(100.f, 100.f, 0.f),
            FVector(100.f, 0.f, 0.f),
    };

    TArray<int32> Triangles = { 0, 1, 3, 1, 2, 3 };
    TArray<FVector2D> UVCoords = {
            FVector2D(0.f, 0.f),
            FVector2D(0.f, 1.f),
            FVector2D(1.f, 1.f),
            FVector2D(1.f, 0.f),
    };

    MeshComponent->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(),
            UVCoords, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}

// Called every frame
void AProcedurallyGeneratedMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

