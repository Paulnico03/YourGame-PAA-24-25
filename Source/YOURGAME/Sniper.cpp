#include "Sniper.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ASniper::ASniper()
{
	UnitName = "Sniper";
}

void ASniper::PerformAction()
{
	Super::PerformAction(); // Chiama la versione della classe base (facoltativo)
	UE_LOG(LogTemp, Warning, TEXT("Lo Sniper attacca da lontano!"));
}
