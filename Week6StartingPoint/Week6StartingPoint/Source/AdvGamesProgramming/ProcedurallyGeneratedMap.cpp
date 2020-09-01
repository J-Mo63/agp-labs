// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcedurallyGeneratedMap.h"
#include "AIManager.h"

// Sets default values
AProcedurallyGeneratedMap::AProcedurallyGeneratedMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh Component"));

    PerlinScale = 1000.0f;
    PerlinRoughness = 0.1f;
    bRegenerateMap = false;
}

// Called when the game starts or when spawned
void AProcedurallyGeneratedMap::BeginPlay()
{
	Super::BeginPlay();

//    Vertices = {
//            FVector(0.f, 0.f, 0.f),
//            FVector(0.f, 100.f, 0.f),
//            FVector(100.f, 100.f, 0.f),
//            FVector(100.f, 0.f, 0.f),
//    };
//
//    Triangles = { 0, 1, 3, 1, 2, 3 };
//
//    UVCoords = {
//            FVector2D(0.f, 0.f),
//            FVector2D(0.f, 1.f),
//            FVector2D(1.f, 1.f),
//            FVector2D(1.f, 0.f),
//    };
//
//    MeshComponent->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(),
//            UVCoords, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}

// Called every frame
void AProcedurallyGeneratedMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bRegenerateMap)
    {
        ClearMap();
        GenerateMap();
        bRegenerateMap = false;
    }
}

bool AProcedurallyGeneratedMap::ShouldTickIfViewportsOnly() const
{
    return true;
}

void AProcedurallyGeneratedMap::GenerateMap()
{
    float PerlinOffset = FMath::RandRange(-10000.0f, 10000.0f);
    for (int Row = 0; Row < Height; Row++)
    {
        for (int Col = 0; Col < Width; Col++)
        {
            float VertexXLocation = float(Col) * GridSize;
            float VertexYLocation = float(Row) * GridSize;
            float Z = FMath::PerlinNoise2D(FVector2D(
                    (float(Col)*PerlinRoughness) + PerlinOffset,
                    (float(Row)*PerlinRoughness) + PerlinOffset)) * PerlinScale;

            Vertices.Add(FVector(VertexXLocation, VertexYLocation, Z));
            UVCoords.Add(FVector2D(Row, Col));
        }
    }

    for (int Row = 0; Row < Height - 1; Row++)
    {
        for (int Col = 0; Col < Width - 1; Col++)
        {
            Triangles.Add((Row * Width) + Col);
            Triangles.Add(((Row+1) * Width) + Col);
            Triangles.Add((Row * Width) + Col+1);
            Triangles.Add(((Row+1) * Width) + Col);
            Triangles.Add(((Row+1) * Width) + Col+1);
            Triangles.Add((Row * Width) + Col+1);
        }
    }

    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVCoords, Normals, Tangents);

    MeshComponent->CreateMeshSection(0, Vertices, Triangles, Normals,
            UVCoords, TArray<FColor>(), Tangents, true);

    if (!AIManager) { return; }
    AIManager->GenerateNodes(Vertices, Width, Height);
}

void AProcedurallyGeneratedMap::ClearMap()
{
    Vertices.Empty();
    Triangles.Empty();
    UVCoords.Empty();
    MeshComponent->ClearAllMeshSections();
}