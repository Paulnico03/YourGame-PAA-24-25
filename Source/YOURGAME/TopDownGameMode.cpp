//TopDownGameMode.cpp
#include "TopDownGameMode.h"
#include "TopDownCameraPawn.h"
#include "Brawler.h"
#include "Sniper.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "UIManager.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

ATopDownGameMode::ATopDownGameMode()
{
	DefaultPawnClass = ATopDownCameraPawn::StaticClass();

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
    
	UE_LOG(LogTemp, Warning, TEXT("🔥 TopDownGameMode::BeginPlay() chiamato!"));

	if (!UIManagerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ UIManagerClass non è impostato nel GameMode!"));
		return;
	}

	// ✅ Protezione contro il nullptr
	UIManagerInstance = CreateWidget<UUIManager>(GetWorld(), UIManagerClass);
	if (!UIManagerInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ UIManager non è stato creato! Assicurati che il Widget Blueprint sia corretto."));
		return;
	}

	UIManagerInstance->AddToViewport();
	UE_LOG(LogTemp, Warning, TEXT("✅ UIManager: Widget creato con successo!"));
}


void ATopDownGameMode::SelectUnit(EUnitType UnitType)
{
	FVector SpawnLocation = FVector(0, 0, 100);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	if (UnitType == EUnitType::Brawler)
	{
		if (!bBrawlerSelected)
		{
			bBrawlerSelected = true;
			UE_LOG(LogTemp, Warning, TEXT("✅ Brawler selezionato!"));

			if (GetWorld())
			{
				ABrawler* Brawler = GetWorld()->SpawnActor<ABrawler>(ABrawler::StaticClass(), SpawnLocation, SpawnRotation);
				if (Brawler)
				{
					UE_LOG(LogTemp, Warning, TEXT("🎮 Brawler creato sulla griglia!"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("❌ ERRORE: Brawler non è stato creato!"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("⚠️ Brawler già selezionato!"));
		}
	}
	else if (UnitType == EUnitType::Sniper)
	{
		if (!bSniperSelected)
		{
			bSniperSelected = true;
			UE_LOG(LogTemp, Warning, TEXT("✅ Sniper selezionato!"));

			if (GetWorld())
			{
				ASniper* Sniper = GetWorld()->SpawnActor<ASniper>(ASniper::StaticClass(), SpawnLocation + FVector(200, 0, 0), SpawnRotation);
				if (Sniper)
				{
					UE_LOG(LogTemp, Warning, TEXT("🎮 Sniper creato sulla griglia!"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("❌ ERRORE: Sniper non è stato creato!"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("⚠️ Sniper già selezionato!"));
		}
	}
}
