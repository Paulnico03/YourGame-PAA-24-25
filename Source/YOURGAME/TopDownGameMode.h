//TopDownGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "UIManager.h" // ✅ Assicuriamoci di includerlo
#include "UnitTypes.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownGameMode.generated.h"

UCLASS()
class YOURGAME_API ATopDownGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATopDownGameMode();

protected:
	virtual void BeginPlay() override;

public:
	// Classe del widget UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUIManager> UIManagerClass;

	// Istanza del widget UIManager
	UPROPERTY()
	UUIManager* UIManagerInstance;

	// Selezione dell'unità
	UFUNCTION(BlueprintCallable, Category = "Unit Selection")
	void SelectUnit(EUnitType UnitType);

private:
	bool bBrawlerSelected = false;
	bool bSniperSelected = false;
};
