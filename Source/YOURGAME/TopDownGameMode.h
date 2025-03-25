//TopDownGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "UIManager.h" // ✅ Assicuriamoci di includerlo
#include "UnitTypes.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownGameMode.generated.h"


class AGridTile;  // Dichiarazione anticipata della classe GridTile
class ABrawler;
class ASniper;

UCLASS()
class YOURGAME_API ATopDownGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATopDownGameMode();
	
	// Funzione per gestire il click sulla griglia
	void OnGridClicked();


protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	class AGridManager* GridManager;
	// Celle attualmente evidenziate per movimento
	UPROPERTY()
	TArray<AGridTile*> HighlightedTiles;
	void HandleUnitClicked(class AUnitBase* ClickedUnit);
	virtual void Tick(float DeltaTime) override;

	void HighlightMovableTiles(class AUnitBase* Unit);

	// Classe del widget UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUIManager> UIManagerClass;
	void HighlightMovementRange(AActor* Unit, bool bHighlight);
	// Istanza del widget UIManager
	UPROPERTY()
	UUIManager* UIManagerInstance;
	UFUNCTION()
	AGridTile* GetTileAtWorldPosition(FVector WorldPosition);
	// Salva l'unità che hai selezionato (Brawler o Sniper)
	UPROPERTY()
	class AUnitBase* SelectedUnit;

	FTimerHandle MovementTimerHandle;
	TArray<AGridTile*> MovementPath;
	AActor* MovingUnit = nullptr;
	int32 CurrentStepIndex = 0;

	UFUNCTION()
	void MoveStep();


	UFUNCTION(BlueprintCallable, Category = "Grid Navigation")
	void HighlightMovementRange(AActor* Unit, int Range, bool bHighlight);

	// Selezione dell'unità
	UFUNCTION(BlueprintCallable, Category = "Unit Selection")
	void SelectUnit(EUnitType UnitType);
	
	void PlaceUnit(EUnitType UnitType, FVector Location, AGridTile* ClickedTile);

	EUnitType SelectedUnitType = EUnitType::None;  // 🔹 Valore predefinito

	bool bBrawlerSelected = false;  // 🔹 Blocca la selezione multipla
	bool bSniperSelected = false;   // 🔹 Blocca la selezione multipla

	AGridTile* GetTileAt(int32 X, int32 Y);

	// Sposta l'unità sulla cella selezionata con il percorso più breve
	void MoveUnit(AActor* Unit, AGridTile* TargetTile);

	// Gestisce il movimento progressivo lungo il percorso
	void MoveAlongPath(AActor* Unit, const TArray<AGridTile*>& Path);

	UPROPERTY(EditAnywhere, Category = "Grid")
	float GridTileSize = 100.0f; // Valore standard per la griglia
	void MoveUnitAlongPath(class AUnitBase* Unit, const TArray<class AGridTile*>& Path);
	bool bBrawlerPlaced = false; // ✅ Indica se un Brawler è stato già piazzato
	bool bSniperPlaced = false;  // ✅ Indica se uno Sniper è stato già piazzato
private:
	ABrawler* BrawlerInstance = nullptr;
	ASniper* SniperInstance = nullptr;

};
