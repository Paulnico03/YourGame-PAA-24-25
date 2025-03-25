//Sniper.h
#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "Sniper.generated.h"

UCLASS()
class YOURGAME_API ASniper : public AUnitBase
{
	GENERATED_BODY()

public:
	ASniper();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;
	virtual void PerformAction() override;

	// 🔹 Gestione Hover del Mouse
	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;
private:
        bool bIsSelected = false; // Indica se l'unità è selezionata

};
