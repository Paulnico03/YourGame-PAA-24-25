//GridTile.cpp
#include "GridTile.h"
#include "UnitBase.h"
#include "TopDownGameMode.h"
#include "UnitTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"


AGridTile::AGridTile()
{
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    PrimaryActorTick.bCanEverTick = false;
    GridX = 0;
    GridY = 0;
    // Creazione della mesh principale
    TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
    RootComponent = TileMesh;
    // Carica i materiali in C++
    static ConstructorHelpers::FObjectFinder<UMaterial> BrawlerMat(TEXT("Material'/Game/Materiale/M_Brawler.M_Brawler'"));
    if (BrawlerMat.Succeeded())
    {
        BrawlerMaterial = BrawlerMat.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Errore: M_Brawler non trovato!"));
    }

    static ConstructorHelpers::FObjectFinder<UMaterial> SniperMat(TEXT("Material'/Game/Materiale/M_Sniper.M_Sniper'"));
    if (SniperMat.Succeeded())
    {
        SniperMaterial = SniperMat.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Errore: M_Sniper non trovato!"));
    }

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
    if (!HighlightMaterial) {
        HighlightMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/M_Highlight"));
    }
    if (!DefaultMaterial) {
        DefaultMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/M_Default"));
    }
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

       // UE_LOG(LogTemp, Warning, TEXT("🚧 Cella (%d, %d) impostata come ostacolo"), XIndex, YIndex);
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

       // UE_LOG(LogTemp, Warning, TEXT("✅ Cella (%d, %d) ripristinata allo stato normale."), XIndex, YIndex);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Errore: impossibile ripristinare la cella (%d, %d), materiale non valido!"), XIndex, YIndex);
    }
}
// Cambia il materiale in base all'unità selezionata
void AGridTile::SetUnitMaterial(EUnitType UnitType)
{
   // UE_LOG(LogTemp, Warning, TEXT("🚀 SetUnitMaterial chiamata su cella (%d, %d) per unità %d"), XIndex, YIndex, static_cast<int32>(UnitType));

    if (!TileMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ TileMesh non valido in SetUnitMaterial!"));
        return;
    }

    // Controlla se la cella è già occupata da un'unità
    if (bIsOccupied)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ La cella (%d, %d) è già occupata, impossibile piazzare l'unità!"), XIndex, YIndex);
        return;
    }

    UMaterialInterface* SelectedMaterial = nullptr;

    if (UnitType == EUnitType::Brawler && BrawlerMaterial)
    {
        SelectedMaterial = BrawlerMaterial;
        UE_LOG(LogTemp, Warning, TEXT("🎨 BrawlerMaterial selezionato!"));
    }
    else if (UnitType == EUnitType::Sniper && SniperMaterial)
    {
        SelectedMaterial = SniperMaterial;
        UE_LOG(LogTemp, Warning, TEXT("🎨 SniperMaterial selezionato!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Nessun materiale valido trovato!"));
        return;
    }

    TileMesh->SetMaterial(0, SelectedMaterial);
    TileMesh->MarkRenderStateDirty();  // 🔥 Forza il rendering

    // ✅ Segna la cella come occupata SOLO ora che l'unità è effettivamente piazzata
    bIsOccupied = true;

    UE_LOG(LogTemp, Warning, TEXT("✅ Unità posizionata e cella (%d, %d) segnata come occupata!"), XIndex, YIndex);
}



void AGridTile::NotifyActorOnClicked(FKey ButtonPressed)
{
    Super::NotifyActorOnClicked(ButtonPressed);

    ATopDownGameMode* GameMode = Cast<ATopDownGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode && GameMode->SelectedUnit)
    {
        if (GameMode->HighlightedTiles.Contains(this)) // ✅ solo se clicchi una cella evidenziata
        {
            UE_LOG(LogTemp, Warning, TEXT("🚶‍♂️ Click su cella evidenziata!"));
            TArray<AGridTile*> SingleTilePath;
            SingleTilePath.Add(this);
            GameMode->MoveUnitAlongPath(GameMode->SelectedUnit, SingleTilePath);

            GameMode->HighlightMovementRange(Cast<AActor>(GameMode->SelectedUnit), 0, false);


            GameMode->SelectedUnit = nullptr;
        }
    }
}



void AGridTile::SetHighlight(bool bHighlight)
{
    UE_LOG(LogTemp, Warning, TEXT("🔆 SetHighlight chiamato su cella (%d, %d). Stato bloccata: %s"),
    XIndex, YIndex, bIsBlocked ? TEXT("TRUE") : TEXT("FALSE"));

    if (!TileMesh) return;

    UMaterialInterface* SelectedMaterial = bHighlight ? HighlightMaterial : GridMaterial_Normal;
    TileMesh->SetMaterial(0, SelectedMaterial);

    UE_LOG(LogTemp, Warning, TEXT("🔆 Highlight %s sulla cella (%d, %d)"), bHighlight ? TEXT("ATTIVATO") : TEXT("DISATTIVATO"), XIndex, YIndex);
}

FVector2D AGridTile::GetGridPosition() const {
    return FVector2D(GridX, GridY); // Assumendo che GridX e GridY siano definiti nella classe
}

void AGridTile::SetHighlighted(bool bHighlight) {
    

    if (!MeshComponent) return;

    UMaterialInterface* MaterialToApply = bHighlight ? HighlightMaterial : DefaultMaterial;

    if (MaterialToApply) {
        MeshComponent->SetMaterial(0, MaterialToApply);
    }
}

