#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTile.h"
#include "GridManager.generated.h"

UCLASS()
class YOURGAME_API AGridManager : public AActor
{
    GENERATED_BODY()

public:
    AGridManager();

protected:
    virtual void BeginPlay() override;
    
    // Funzione per ottenere le celle adiacenti
    TArray<AGridTile*> GetAdjacentTiles(AGridTile* Tile) const;

    // 🔥 Controlla se una regione è completamente circondata da ostacoli
    bool CheckFullySurroundedRegion(AGridTile* StartTile, TArray<AGridTile*>& Region, TSet<AGridTile*>& VisitedTiles);

    // 🔥 Corregge le regioni isolate sostituendo celle libere con ostacoli e bilanciando la percentuale di ostacoli
    void FixIsolatedRegions();

    // 🔥 Rimuove casualmente ostacoli per mantenere il numero totale sotto la percentuale specificata
    void RemoveRandomObstacles(int32 NumToRemove);

private:
    TArray<AGridTile*> GridTiles;
    int32 NumObstacles; // Numero di ostacoli nella mappa

public:
    UPROPERTY(EditAnywhere, Category = "Grid")
    int32 GridSizeX = 25;

    UPROPERTY(EditAnywhere, Category = "Grid")
    int32 GridSizeY = 25;

    UPROPERTY(EditAnywhere, Category = "Grid")
    float TileSize = 105.0f;

    UPROPERTY(EditAnywhere, Category = "Grid")
    float ObstaclePercentage = 20.0f;

    // Genera la griglia e gli ostacoli intelligenti
    void GenerateGrid();
    void GenerateSmartObstacles();
};
