#include "Brawler.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ABrawler::ABrawler()
{
	UnitName = "Brawler";
}

void ABrawler::PerformAction()
{
	Super::PerformAction(); // Chiama la versione della classe base (facoltativo)
	UE_LOG(LogTemp, Warning, TEXT("Il Brawler attacca da vicino!"));
}
