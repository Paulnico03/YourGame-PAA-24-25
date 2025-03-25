//Brawler.cpp
#include "Brawler.h"
#include "Components/StaticMeshComponent.h"
#include "GridTile.h"
#include "TopDownGameMode.h"
#include "Kismet/GameplayStatics.h"





ABrawler::ABrawler()
{
	PrimaryActorTick.bCanEverTick = false;

	// Creazione del MeshComponent
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Meshes/SM_Brawler.SM_Brawler"));
	if (MeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshAsset.Object);
		MeshComponent->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));// 🔹 Riduci la scala
		//MeshComponent->SetRelativeLocation(FVector(0.f, -5.f, 0.f)); // Sposta la mesh verso il basso
		UE_LOG(LogTemp, Warning, TEXT("✅ SM_Brawler assegnata e scalata correttamente!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Errore: Static Mesh SM_Brawler non trovata!"));
	}


	// Imposta le collisioni per rilevare il passaggio del mouse
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECC_Pawn);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

	// Abilita eventi di hover
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->OnBeginCursorOver.AddDynamic(this, &ABrawler::OnMouseEnter);
	MeshComponent->OnEndCursorOver.AddDynamic(this, &ABrawler::OnMouseExit);
}



void ABrawler::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("✅ Brawler Spawnato!"));
	MeshComponent->SetCollisionObjectType(ECC_Pawn);
}

void ABrawler::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

	UE_LOG(LogTemp, Warning, TEXT("🖱️ Brawler selezionato! Blocco il percorso."));

	ATopDownGameMode* GameMode = Cast<ATopDownGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->SelectedUnit = this;

		// Blocca il percorso evidenziato da adesso
		GameMode->HighlightMovableTiles(this);
	}
}




void ABrawler::NotifyActorEndCursorOver()
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


void ABrawler::NotifyActorBeginCursorOver()
{
	UE_LOG(LogTemp, Warning, TEXT("⚠️ NotifyActorBeginCursorOver ATTIVATO!"));

	Super::NotifyActorBeginCursorOver();

	ATopDownGameMode* GameMode = Cast<ATopDownGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("🖱️ Mouse sopra il Brawler!"));
		GameMode->HighlightMovementRange(this, 6, true);
	}
}


void ABrawler::OnMouseEnter(UPrimitiveComponent* TouchedComponent)
{
	FVector MousePosition;
	FVector MouseDirection;
    
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->DeprojectMousePositionToWorld(MousePosition, MouseDirection);
		//UE_LOG(LogTemp, Warning, TEXT("🔥 Mouse sopra il Brawler! Posizione Mouse: %s"), *MousePosition.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ PlayerController non trovato!"));
	}
}


void ABrawler::OnMouseExit(UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("❄️ Mouse uscito dal Brawler!"));
}
void ABrawler::MoveAlongPath(const TArray<AGridTile*>& InPath)
{
	Path = InPath;
	if (InPath.Num() == 0) return;

	FVector TargetLocation = InPath.Last()->GetActorLocation();
	SetActorLocation(TargetLocation);
	CurrentTile = InPath.Last();
}
