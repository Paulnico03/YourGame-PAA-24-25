//GridTile.cpp
#include "GridTile.h"
#include "Components/StaticMeshComponent.h"

AGridTile::AGridTile()
{
    PrimaryActorTick.bCanEverTick = false;

    // Creazione della mesh principale
    TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
    RootComponent = TileMesh;

    // Imposta la static mesh per la tile
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Plane"));
    if (MeshAsset.Succeeded())
    {
        TileMesh->SetStaticMesh(MeshAsset.Object);
    }

    // Carica il materiale per le celle normali
    static ConstructorHelpers::FObjectFinder<UMaterial> NormalMaterial(TEXT("Material'/Game/Materiale/GridMaterial_Normal.GridMaterial_Normal'"));
    if (NormalMaterial.Succeeded())
    {
        GridMaterial_Normal = NormalMaterial.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Errore: Materiale normale non trovato!"));
    }

    // Creazione della mesh per l'ostacolo
    ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObstacleMesh"));
    ObstacleMesh->SetupAttachment(RootComponent);
    ObstacleMesh->SetRelativeLocation(FVector(0, 0, 1.0f));
    ObstacleMesh->SetVisibility(false);

    // Imposta la mesh dell'ostacolo
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ObstacleAsset(TEXT("/Engine/BasicShapes/Plane"));
    if (ObstacleAsset.Succeeded())
    {
        ObstacleMesh->SetStaticMesh(ObstacleAsset.Object);
        ObstacleMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 0.1f));
    }

    // Carica i materiali degli ostacoli
    static ConstructorHelpers::FObjectFinder<UMaterial> Material1(TEXT("Material'/Game/Materiale/M_Ostacolo1.M_Ostacolo1'"));
    if (Material1.Succeeded())
    {
        ObstacleMaterial1 = Material1.Object;
    }

    static ConstructorHelpers::FObjectFinder<UMaterial> Material2(TEXT("Material'/Game/Materiale/M_Ostacolo2.M_Ostacolo2'"));
    if (Material2.Succeeded())
    {
        ObstacleMaterial2 = Material2.Object;
    }
}

void AGridTile::BeginPlay()
{
    Super::BeginPlay();

    if (!IsValid(GridMaterial_Normal))
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ERRORE: GridMaterial_Normal non caricato!"));
    }
}

void AGridTile::SetGridPosition(int32 X, int32 Y)
{
    XIndex = X;
    YIndex = Y;

    // Imposta la tile come normale se non è un ostacolo
    if (!bIsObstacle && IsValid(TileMesh) && IsValid(GridMaterial_Normal))
    {
        TileMesh->SetMaterial(0, GridMaterial_Normal);
    }
}

void AGridTile::SetAsObstacle()
{
    if (IsValid(TileMesh))
    {
        bIsObstacle = true;
        TileMesh->SetMaterial(0, GridMaterial);

        // Imposta una texture casuale per l'ostacolo
        if (IsValid(ObstacleMesh) && (IsValid(ObstacleMaterial1) || IsValid(ObstacleMaterial2)))
        {
            ObstacleMesh->SetMaterial(0, FMath::RandBool() ? ObstacleMaterial1 : ObstacleMaterial2);
            ObstacleMesh->SetVisibility(true);
        }

        UE_LOG(LogTemp, Warning, TEXT("🚧 Cella (%d, %d) impostata come ostacolo"), XIndex, YIndex);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Errore: TileMesh non valido in SetAsObstacle()!"));
    }
}

void AGridTile::SetAsNormal()
{
    if (IsValid(TileMesh) && IsValid(GridMaterial_Normal))
    {
        bIsObstacle = false;
        TileMesh->SetMaterial(0, GridMaterial_Normal);

        // Nasconde l'ostacolo se esiste
        if (IsValid(ObstacleMesh))
        {
            ObstacleMesh->SetVisibility(false);
        }

        UE_LOG(LogTemp, Warning, TEXT("✅ Cella (%d, %d) ripristinata allo stato normale."), XIndex, YIndex);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Errore: impossibile ripristinare la cella (%d, %d), materiale non valido!"), XIndex, YIndex);
    }
}
