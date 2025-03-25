
//TopDownGameMode.
#include "UnitBase.h"
#include "TopDownGameMode.h"
#include "MyPlayerController.h"
#include "Brawler.h"
#include "TopDownCameraPawn.h"
#include "GridTile.h"
#include "Sniper.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "UIManager.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h" // Aggiunto per usare TActorIterator


ATopDownGameMode::ATopDownGameMode()
{
	PlayerControllerClass = AMyPlayerController::StaticClass();


	DefaultPawnClass = ATopDownCameraPawn::StaticClass();
	SelectedUnitType = EUnitType::Brawler; // Default
	PrimaryActorTick.bCanEverTick = true;

	// 🔹 Carica la classe del widget UI
	static ConstructorHelpers::FClassFinder<UUIManager> UIManagerBPClass(TEXT("/Game/UI/WBP_CharacterSelection"));
	if (UIManagerBPClass.Succeeded())
	{
		UIManagerClass = UIManagerBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("✅ UIManagerClass caricato correttamente!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ UIManagerClass non trovato! Controlla il percorso."));
	}
}

void ATopDownGameMode::BeginPlay()
{
	Super::BeginPlay();
	

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;

		PC->InputComponent->BindAction("LeftClick", IE_Pressed, this, &ATopDownGameMode::OnGridClicked);
	}
	UE_LOG(LogTemp, Warning, TEXT("🔥 TopDownGameMode::BeginPlay() chiamato!"));

	if (!UIManagerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ UIManagerClass non è impostato nel GameMode!"));
		return;
	}

	UIManagerInstance = CreateWidget<UUIManager>(GetWorld(), UIManagerClass);
	if (!UIManagerInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ UIManager non è stato creato!"));
		return;
	}

	UIManagerInstance->AddToViewport();
	UE_LOG(LogTemp, Warning, TEXT("✅ UIManager: Widget creato con successo!"));
}


void ATopDownGameMode::SelectUnit(EUnitType UnitType)
{
	if (UnitType == EUnitType::Brawler)
	{
		if (bBrawlerSelected) // Controlla se è già stato selezionato
		{
			UE_LOG(LogTemp, Warning, TEXT("⚠️ Brawler già posizionato, non puoi selezionarlo di nuovo!"));
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("✅ Unità selezionata: Brawler"));
	}
	else if (UnitType == EUnitType::Sniper)
	{
		if (bSniperSelected) // Controlla se è già stato selezionato
		{
			UE_LOG(LogTemp, Warning, TEXT("⚠️ Sniper già posizionato, non puoi selezionarlo di nuovo!"));
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("✅ Unità selezionata: Sniper"));
	}
    
	SelectedUnitType = UnitType;
}



void ATopDownGameMode::PlaceUnit(EUnitType UnitType, FVector Location, AGridTile* ClickedTile)
{
    if (!ClickedTile)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ClickedTile non valido in PlaceUnit!"));
        return;
    }

    // 🔹 Verifica se la cella è un ostacolo
    if (ClickedTile->bIsObstacle)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ Tentativo di piazzare un'unità su un ostacolo alla cella (%d, %d), operazione annullata!"), ClickedTile->XIndex, ClickedTile->YIndex);
        return; // ⛔ Evita di segnare l'unità come posizionata
    }

    UE_LOG(LogTemp, Warning, TEXT("🔥 Tentativo di piazzare un'unità sulla cella (%d, %d)!"), ClickedTile->XIndex, ClickedTile->YIndex);

    if ((UnitType == EUnitType::Brawler && bBrawlerPlaced) ||
        (UnitType == EUnitType::Sniper && bSniperPlaced))
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ Questa unità è già stata posizionata e non può essere piazzata di nuovo!"));
        return;
    }

    // ✅ Centra il posizionamento dell'unità nella cella
	FVector SpawnLocation = ClickedTile->GetActorLocation();
	SpawnLocation.Z += 50;


    FRotator SpawnRotation = FRotator::ZeroRotator;

    UWorld* World = GetWorld();
    if (!World) return;

	if (UnitType == EUnitType::Brawler)
	{
		ABrawler* Brawler = World->SpawnActor<ABrawler>(ABrawler::StaticClass(), SpawnLocation, SpawnRotation);
		if (Brawler)
		{
			BrawlerInstance = Brawler; // 🔴 Memorizza l'istanza
			bBrawlerPlaced = true;
			bBrawlerSelected = true;
			
			UE_LOG(LogTemp, Warning, TEXT("🎮 Brawler posizionato sulla griglia!"));
		}
	}
	else if (UnitType == EUnitType::Sniper)
	{
		ASniper* Sniper = World->SpawnActor<ASniper>(ASniper::StaticClass(), SpawnLocation, SpawnRotation);
		if (Sniper)
		{
			SniperInstance = Sniper; // 🔴 Memorizza l'istanza
			bSniperPlaced = true;
			bSniperSelected = true;
			UE_LOG(LogTemp, Warning, TEXT("🎮 Sniper posizionato sulla griglia!"));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("📌 Stato bIsBlocked della cella di piazzamento (%d, %d): %s"),
		ClickedTile->XIndex, ClickedTile->YIndex, ClickedTile->bIsBlocked ? TEXT("TRUE") : TEXT("FALSE"));

    // 🔴 **NON CHIAMARE** ClickedTile->SetUnitMaterial(UnitType);
}



void ATopDownGameMode::OnGridClicked()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	FHitResult Hit;
	PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		AUnitBase* ClickedUnit = Cast<AUnitBase>(Hit.GetActor());
		if (ClickedUnit)
		{
			UE_LOG(LogTemp, Warning, TEXT("🖱️ Hai cliccato sull'unità: %s"), *ClickedUnit->GetName());

			if (ClickedUnit->IsA(ABrawler::StaticClass()))
			{
				bBrawlerSelected = true;
				HighlightMovementRange(ClickedUnit, Cast<ABrawler>(ClickedUnit)->GetMovementRange(), true);
			}
			else if (ClickedUnit->IsA(ASniper::StaticClass()))
			{
				bSniperSelected = true;
				HighlightMovementRange(ClickedUnit, Cast<ASniper>(ClickedUnit)->GetMovementRange(), true);
			}

			return; // ⛔ Interrompi qui, così non viene processato come clic su cella
		}

		AGridTile* ClickedTile = Cast<AGridTile>(Hit.GetActor());
		if (!ClickedTile) return;

		// ✅ Se sto piazzando il Brawler
		if (SelectedUnitType == EUnitType::Brawler && !bBrawlerPlaced)
		{
			PlaceUnit(EUnitType::Brawler, ClickedTile->GetActorLocation(), ClickedTile);
			return;
		}

		// ✅ Se sto piazzando lo Sniper
		if (SelectedUnitType == EUnitType::Sniper && !bSniperPlaced)
		{
			PlaceUnit(EUnitType::Sniper, ClickedTile->GetActorLocation(), ClickedTile);
			return;
		}

		// 🔹 Se clicco di nuovo sul Brawler già posizionato
		if (BrawlerInstance && ClickedTile == GetTileAtWorldPosition(BrawlerInstance->GetActorLocation()))
		{
			UE_LOG(LogTemp, Warning, TEXT("🖱️ Brawler selezionato! Blocco il percorso."));
			bBrawlerSelected = true;
			HighlightMovementRange(BrawlerInstance, BrawlerInstance->GetMovementRange(), true);
			return;
		}

		// 🔹 Se clicco di nuovo sullo Sniper già posizionato
		if (SniperInstance && ClickedTile == GetTileAtWorldPosition(SniperInstance->GetActorLocation()))
		{
			UE_LOG(LogTemp, Warning, TEXT("🖱️ Sniper selezionato! Blocco il percorso."));
			bSniperSelected = true;
			HighlightMovementRange(SniperInstance, SniperInstance->GetMovementRange(), true);
			return;
		}

		// ✅ Se un'unità è selezionata e ho cliccato una cella evidenziata
		if (bBrawlerSelected && BrawlerInstance && HighlightedTiles.Contains(ClickedTile))
		{
			MoveUnit(BrawlerInstance, ClickedTile);
			return;
		}
		if (bSniperSelected && SniperInstance && HighlightedTiles.Contains(ClickedTile))
		{
			MoveUnit(SniperInstance, ClickedTile);
			return;
		}
	}
}




void ATopDownGameMode::HighlightMovementRange(AActor* Unit, int Range, bool bHighlight)
{
    if (!Unit) return;

    // 🔁 Pulizia precedente (reset)
    if (!bHighlight)
    {
        for (AGridTile* Tile : HighlightedTiles)
        {
            if (Tile)
            {
                Tile->SetHighlight(false);
                Tile->bIsBlocked = false;
                //UE_LOG(LogTemp, Warning, TEXT("🔓 Cella sbloccata: (%d, %d)"), Tile->XIndex, Tile->YIndex);
            }
        }
        HighlightedTiles.Empty();
        return;
    }

    FVector UnitLocation = Unit->GetActorLocation();
    AGridTile* StartTile = this->GetTileAtWorldPosition(UnitLocation);

    if (!StartTile)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ StartTile non valido!"));
        return;
    }

    StartTile->bIsBlocked = true;
   // UE_LOG(LogTemp, Warning, TEXT("🔒 Cella iniziale bloccata: (%d, %d)"), StartTile->XIndex, StartTile->YIndex);

    int32 MaxSteps = Range;
    TQueue<TPair<AGridTile*, int32>> TileQueue;
    TSet<AGridTile*> VisitedTiles;

    TileQueue.Enqueue(TPair<AGridTile*, int32>(StartTile, 0));
    VisitedTiles.Add(StartTile);

    while (!TileQueue.IsEmpty())
    {
        TPair<AGridTile*, int32> CurrentPair;
        TileQueue.Dequeue(CurrentPair);

        AGridTile* CurrentTile = CurrentPair.Key;
        int32 Steps = CurrentPair.Value;

        if (CurrentTile && Steps <= MaxSteps)
        {
            CurrentTile->SetHighlight(true);
            HighlightedTiles.Add(CurrentTile);
            //UE_LOG(LogTemp, Warning, TEXT("🔆 Highlight su (%d, %d) — Bloccata: %s"), CurrentTile->XIndex, CurrentTile->YIndex, CurrentTile->bIsBlocked ? TEXT("TRUE") : TEXT("FALSE"));

            if (Steps >= MaxSteps) continue;

            TArray<AGridTile*> Neighbors;
            Neighbors.Add(GetTileAt(CurrentTile->XIndex + 1, CurrentTile->YIndex));
            Neighbors.Add(GetTileAt(CurrentTile->XIndex - 1, CurrentTile->YIndex));
            Neighbors.Add(GetTileAt(CurrentTile->XIndex, CurrentTile->YIndex + 1));
            Neighbors.Add(GetTileAt(CurrentTile->XIndex, CurrentTile->YIndex - 1));

            for (AGridTile* Neighbor : Neighbors)
            {
                if (Neighbor && !Neighbor->bIsObstacle && !Neighbor->bIsBlocked && !VisitedTiles.Contains(Neighbor))
                {
                    TileQueue.Enqueue(TPair<AGridTile*, int32>(Neighbor, Steps + 1));
                    VisitedTiles.Add(Neighbor);
                    UE_LOG(LogTemp, Warning, TEXT("✅ Cella libera aggiunta: (%d, %d)"), Neighbor->XIndex, Neighbor->YIndex);
                }
                else if (Neighbor && Neighbor->bIsBlocked)
                {
                    UE_LOG(LogTemp, Warning, TEXT("⛔️ Cella già bloccata ignorata: (%d, %d)"), Neighbor->XIndex, Neighbor->YIndex);
                }
                else if (Neighbor && Neighbor->bIsObstacle)
                {
                    UE_LOG(LogTemp, Warning, TEXT("🚧 Cella ostacolo ignorata: (%d, %d)"), Neighbor->XIndex, Neighbor->YIndex);
                }
            }
        }
    }
}





AGridTile* ATopDownGameMode::GetTileAtWorldPosition(FVector WorldPosition)
{
	AGridTile* NearestTile = nullptr;
	float MinDistance = FLT_MAX;

	for (TActorIterator<AGridTile> It(GetWorld()); It; ++It)
	{
		float Distance = FVector::Dist(It->GetActorLocation(), WorldPosition);
        
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NearestTile = *It;
		}
	}

	if (NearestTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("📌 Trovata cella più vicina a (%s) → (%d, %d)"), 
			*WorldPosition.ToString(), NearestTile->XIndex, NearestTile->YIndex);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Nessuna cella trovata vicino a (%s)!"), *WorldPosition.ToString());
	}

	return NearestTile;
}








AGridTile* ATopDownGameMode::GetTileAt(int32 X, int32 Y)
{
	for (TActorIterator<AGridTile> It(GetWorld()); It; ++It)
	{
		if (It->XIndex == X && It->YIndex == Y)
		{
			return *It;
		}
	}
	return nullptr;
}

void ATopDownGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	FHitResult Hit;
	PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("🎯 Oggetto sotto il mouse: %s"), *Hit.GetActor()->GetName());
	}
}

void ATopDownGameMode::MoveUnit(AActor* Unit, AGridTile* TargetTile)
{
	if (!Unit || !TargetTile) return;

	AGridTile* StartTile = GetTileAtWorldPosition(Unit->GetActorLocation());
	if (!StartTile)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Nessuna cella di partenza trovata!"));
		return;
	}

	// Algoritmo per trovare il percorso più corto (BFS)
	TQueue<TPair<AGridTile*, int32>> TileQueue;
	TMap<AGridTile*, AGridTile*> CameFrom;
    
	TileQueue.Enqueue(TPair<AGridTile*, int32>(StartTile, 0));
	CameFrom.Add(StartTile, nullptr);

	while (!TileQueue.IsEmpty())
	{
		TPair<AGridTile*, int32> CurrentPair;
		TileQueue.Dequeue(CurrentPair);

		AGridTile* CurrentTile = CurrentPair.Key;

		if (CurrentTile == TargetTile)
		{
			// Abbiamo trovato il percorso più breve, ricostruiamolo
			TArray<AGridTile*> Path;
			AGridTile* Step = TargetTile;
			while (Step)
			{
				Path.Insert(Step, 0);
				Step = CameFrom[Step];
			}

			// Muove l'unità lungo il percorso
			MoveAlongPath(Unit, Path);
			return;
		}

		TArray<AGridTile*> Neighbors;
		Neighbors.Add(GetTileAt(CurrentTile->XIndex + 1, CurrentTile->YIndex));
		Neighbors.Add(GetTileAt(CurrentTile->XIndex - 1, CurrentTile->YIndex));
		Neighbors.Add(GetTileAt(CurrentTile->XIndex, CurrentTile->YIndex + 1));
		Neighbors.Add(GetTileAt(CurrentTile->XIndex, CurrentTile->YIndex - 1));

		for (AGridTile* Neighbor : Neighbors)
		{
			if (Neighbor && !Neighbor->bIsObstacle && !CameFrom.Contains(Neighbor))
			{
				TileQueue.Enqueue(TPair<AGridTile*, int32>(Neighbor, CurrentPair.Value + 1));
				CameFrom.Add(Neighbor, CurrentTile);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("❌ Nessun percorso trovato!"));
}

void ATopDownGameMode::MoveAlongPath(AActor* Unit, const TArray<AGridTile*>& Path)
{
	if (!Unit || Path.Num() == 0) return;

	AUnitBase* RealUnit = Cast<AUnitBase>(Unit);
	if (RealUnit)
	{
		RealUnit->StartMovingAlongPath(Path);
	}

	// Dopo aver avviato il movimento, resettiamo gli highlight
	HighlightMovementRange(Unit, 0, false);

	if (Unit->IsA(ABrawler::StaticClass()))
	{
		bBrawlerSelected = false;
	}
	else if (Unit->IsA(ASniper::StaticClass()))
	{
		bSniperSelected = false;
	}
}



void ATopDownGameMode::HighlightMovableTiles(AUnitBase* Unit)
{
	if (!Unit) return;
	// TODO: Evidenzia celle intorno a Unit->CurrentTile
}
void ATopDownGameMode::MoveUnitAlongPath(AUnitBase* Unit, const TArray<AGridTile*>& Path)
{
	if (Unit)
	{
		Unit->MoveAlongPath(Path);
	}
}
void ATopDownGameMode::HandleUnitClicked(AUnitBase* ClickedUnit)
{
	if (!ClickedUnit) return;

	// Se è un Brawler
	if (ClickedUnit == BrawlerInstance)
	{
		bBrawlerSelected = true;
		bSniperSelected = false;
		UE_LOG(LogTemp, Warning, TEXT("🖱️ Brawler selezionato! Blocco il percorso."));
		HighlightMovementRange(BrawlerInstance, BrawlerInstance->GetMovementRange(), true);
	}

	// Se è uno Sniper
	else if (ClickedUnit == SniperInstance)
	{
		bSniperSelected = true;
		bBrawlerSelected = false;
		UE_LOG(LogTemp, Warning, TEXT("🖱️ Sniper selezionato! Blocco il percorso."));
		HighlightMovementRange(SniperInstance, SniperInstance->GetMovementRange(), true);
	}
}
void ATopDownGameMode::MoveStep()
{
	if (!MovingUnit || CurrentStepIndex >= MovementPath.Num())
	{
		GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);

		// Spegni le celle illuminate dopo il movimento
		HighlightMovementRange(MovingUnit, 0, false);

		// Deseleziona l'unità dopo il movimento
		if (MovingUnit->IsA(ABrawler::StaticClass()))
			bBrawlerSelected = false;
		else if (MovingUnit->IsA(ASniper::StaticClass()))
			bSniperSelected = false;

		// Reset
		MovementPath.Empty();
		MovingUnit = nullptr;
		CurrentStepIndex = 0;
		return;
	}

	FVector TargetLocation = MovementPath[CurrentStepIndex]->GetActorLocation();
	TargetLocation.Z += 50;
	MovingUnit->SetActorLocation(TargetLocation);

	CurrentStepIndex++;
}
