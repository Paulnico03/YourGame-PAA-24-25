//GridManager.cpp
#include "GridManager.h"
#include "Engine/World.h"
#include "Containers/Queue.h"

AGridManager::AGridManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGridManager::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("🚀 Inizio della generazione della griglia"));

    GenerateGrid();
    GenerateSmartObstacles();
}

// Creazione della griglia
void AGridManager::GenerateGrid()
{
    for (int32 X = 0; X < GridSizeX; X++)
    {
        for (int32 Y = 0; Y < GridSizeY; Y++)
        {
            FVector TileLocation = FVector(X * TileSize, Y * TileSize, 0);
            FRotator TileRotation = FRotator::ZeroRotator;

            AGridTile* NewTile = GetWorld()->SpawnActor<AGridTile>(AGridTile::StaticClass(), TileLocation, TileRotation);
            if (NewTile)
            {
                NewTile->SetGridPosition(X, Y);
                GridTiles.Add(NewTile);
            }
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("✅ Griglia creata con successo!"));
}

void AGridManager::GenerateSmartObstacles()
{
    UE_LOG(LogTemp, Warning, TEXT("🌲 Generazione ostacoli intelligente..."));

    int32 NumObstaclesToPlace = FMath::RoundToInt((ObstaclePercentage / 100.0f) * GridTiles.Num());
    int32 ObstaclesPlaced = 0;

    TArray<AGridTile*> AvailableTiles;

    // Raccogliamo solo le celle che NON sono già parte del percorso connesso
    for (AGridTile* Tile : GridTiles)
    {
        if (!Tile->bIsObstacle)
        {
            AvailableTiles.Add(Tile);
        }
    }

    // Creiamo percorsi principali che devono rimanere sempre aperti
    TArray<AGridTile*> MainPath;
    AGridTile* StartTile = GridTiles[0];  // Angolo in alto a sinistra
    AGridTile* EndTile = GridTiles.Last(); // Angolo in basso a destra

    MainPath.Add(StartTile);
    AGridTile* CurrentTile = StartTile;

    // Creiamo un percorso garantito che attraversa la griglia
    while (CurrentTile != EndTile)
    {
        TArray<AGridTile*> Neighbors = GetAdjacentTiles(CurrentTile);
        Neighbors.RemoveAll([&](AGridTile* Tile) { return MainPath.Contains(Tile); });

        if (Neighbors.Num() > 0)
        {
            CurrentTile = Neighbors[FMath::RandRange(0, Neighbors.Num() - 1)];
            MainPath.Add(CurrentTile);
        }
        else
        {
            break;
        }
    }

    // Proteggiamo il percorso principale dagli ostacoli
    for (AGridTile* Tile : MainPath)
    {
        Tile->bIsObstacle = false;
    }

    // Creiamo blocchi di ostacoli mantenendo sempre una connessione libera
    while (ObstaclesPlaced < NumObstaclesToPlace && AvailableTiles.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, AvailableTiles.Num() - 1);
        AGridTile* StartObstacle = AvailableTiles[RandomIndex];

        if (!StartObstacle || StartObstacle->bIsObstacle) continue;

        TArray<AGridTile*> Block;
        Block.Add(StartObstacle);

        int32 BlockSize = FMath::RandRange(2, 5);  // Blocchi di 2-5 celle
        CurrentTile = StartObstacle;

        for (int32 i = 0; i < BlockSize; i++)
        {
            TArray<AGridTile*> Neighbors = GetAdjacentTiles(CurrentTile);
            Neighbors.RemoveAll([&](AGridTile* Tile) { return Block.Contains(Tile) || Tile->bIsObstacle; });

            if (Neighbors.Num() > 0)
            {
                CurrentTile = Neighbors[FMath::RandRange(0, Neighbors.Num() - 1)];
                Block.Add(CurrentTile);
            }
            else
            {
                break; // Fermiamo il blocco se non ci sono più celle disponibili
            }
        }

        // Impostiamo il blocco come ostacolo
        for (AGridTile* Tile : Block)
        {
            if (ObstaclesPlaced < NumObstaclesToPlace)
            {
                Tile->SetAsObstacle();
                AvailableTiles.Remove(Tile);
                ObstaclesPlaced++;
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ Ostacoli generati con successo in modo intelligente!"));

    // 🔥 **Fase Finale: Rimuovere celle isolate (completamente circondate) 🔥
    TArray<AGridTile*> IsolatedCells;
    for (AGridTile* Tile : GridTiles)
    {
        if (!Tile->bIsObstacle)
        {
            TArray<AGridTile*> Neighbors = GetAdjacentTiles(Tile);
            bool AllObstacles = true;

            for (AGridTile* Neighbor : Neighbors)
            {
                if (!Neighbor->bIsObstacle)
                {
                    AllObstacles = false;
                    break;
                }
            }

            if (AllObstacles)
            {
                IsolatedCells.Add(Tile);
            }
        }
    }

    // Convertiamo le celle isolate in ostacoli
    for (AGridTile* Tile : IsolatedCells)
    {
        Tile->SetAsObstacle();
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ Celle isolate convertite in ostacoli."));
}




TArray<AGridTile*> AGridManager::GetAdjacentTiles(AGridTile* Tile) const
{
    TArray<AGridTile*> Neighbors;

    if (!Tile) return Neighbors;

    for (AGridTile* OtherTile : GridTiles)
    {
        if (OtherTile &&
            FMath::Abs(OtherTile->XIndex - Tile->XIndex) + FMath::Abs(OtherTile->YIndex - Tile->YIndex) == 1)
        {
            Neighbors.Add(OtherTile);
        }
    }

    return Neighbors;
}
