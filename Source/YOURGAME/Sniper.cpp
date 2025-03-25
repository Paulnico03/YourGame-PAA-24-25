#include "Sniper.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TopDownGameMode.h"
#include "Kismet/GameplayStatics.h"


ASniper::ASniper()
{
	UnitName = "Sniper";

	// Creazione della mesh dello Sniper
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Meshes/SM_Sniper.SM_Sniper"));
	if (MeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshAsset.Object);
		MeshComponent->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f)); // Riduci la scala
		MeshComponent->SetRelativeLocation(FVector(0.f, -5.f, 0.f)); // Sposta la mesh verso il basso
		UE_LOG(LogTemp, Warning, TEXT("✅ SM_Sniper assegnata e scalata correttamente!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Errore: Static Mesh SM_Sniper non trovata!"));
	}

	// Imposta le collisioni per il mouse hover
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECC_Pawn);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
}

// ✅ Implementazione della funzione `PerformAction`
void ASniper::PerformAction()
{
	UE_LOG(LogTemp, Warning, TEXT("🔫 Lo Sniper sta attaccando da lontano!"));
}
// 🔹 Illuminazione delle celle quando il mouse passa sopra
void ASniper::NotifyActorBeginCursorOver()
{
	UE_LOG(LogTemp, Warning, TEXT("🎯 Mouse sopra lo Sniper!"));

	ATopDownGameMode* GameMode = Cast<ATopDownGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->HighlightMovementRange(this, 10, true); // Sniper ha raggio 10
	}
}

// 🔹 Spegnimento illuminazione quando il mouse esce
void ASniper::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();

	ATopDownGameMode* GameMode = Cast<ATopDownGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		// ❌ NON disattivare l’highlight se l’unità è selezionata
		if (GameMode->SelectedUnit != this)
		{
			GameMode->HighlightMovementRange(this, 0, false);
			UE_LOG(LogTemp, Warning, TEXT("🧹 Highlight rimosso (mouse uscito dall'unità NON selezionata)"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("🔒 Mouse uscito MA unità ancora selezionata → highlight mantenuto"));
		}
	}
}

void ASniper::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

	UE_LOG(LogTemp, Warning, TEXT("🖱️ Sniper selezionato! Blocco il percorso."));

	ATopDownGameMode* GameMode = Cast<ATopDownGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->SelectedUnit = this;

		// Blocca il percorso evidenziato da adesso
		GameMode->HighlightMovableTiles(this);
	}
}

