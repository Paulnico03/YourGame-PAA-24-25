//UnitBase.cpp
#include "UnitBase.h"


AUnitBase::AUnitBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Path = TArray<AGridTile*>(); // inizializza il path
}

void AUnitBase::BeginPlay()
{
	Super::BeginPlay();
}

void AUnitBase::PerformAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Azione eseguita da %s"), *UnitName);
}
void AUnitBase::MoveAlongPath(const TArray<AGridTile*>& InPath)
{
	if (InPath.Num() == 0) return;
	SetActorLocation(InPath.Last()->GetActorLocation());
	CurrentTile = InPath.Last();
	Path = InPath; // salva il percorso se serve
}

void AUnitBase::StartMovingAlongPath(const TArray<AGridTile*>& InPath)
{
	if (Path.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("🚫 Nessun percorso da seguire."));
		return;
	}

	PathToFollow = Path;
	CurrentPathIndex = 0;

	UE_LOG(LogTemp, Warning, TEXT("🚶‍♂️ Inizio movimento step-by-step. Numero celle: %d"), PathToFollow.Num());

	// Inizia il primo passo
	MoveStep();
}

void AUnitBase::MoveStep()
{
	if (CurrentPathIndex >= PathToFollow.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("✅ Movimento completato."));
		GetWorldTimerManager().ClearTimer(MovementTimerHandle);
		return;
	}

	AGridTile* NextTile = PathToFollow[CurrentPathIndex];
	if (!NextTile)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Tile nullo al passo %d"), CurrentPathIndex);
		return;
	}

	FVector TargetLocation = NextTile->GetActorLocation();
	TargetLocation.Z += 50.0f;

	SetActorLocation(TargetLocation);

	UE_LOG(LogTemp, Warning, TEXT("➡️ Mosso a (%d, %d)"), NextTile->XIndex, NextTile->YIndex);

	CurrentPathIndex++;

	// Aspetta 0.2 secondi prima del prossimo passo
	GetWorldTimerManager().SetTimer(
		MovementTimerHandle,
		this,
		&AUnitBase::MoveStep,
		0.2f,
		false
	);
}
