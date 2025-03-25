//Brawler.h
#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "GameFramework/Actor.h"
#include "Brawler.generated.h"

class UStaticMeshComponent; // Dichiarazione avanti per evitare problemi di inclusione

UCLASS()
class YOURGAME_API ABrawler : public AUnitBase
{
	GENERATED_BODY()

public:
	ABrawler();

protected:
	virtual void BeginPlay() override;

public:
	/** Mesh del Brawler */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

	
	// Evento quando il mouse passa sopra il Brawler
	virtual void NotifyActorBeginCursorOver() override;

	// Evento quando il mouse esce dal Brawler
	virtual void NotifyActorEndCursorOver() override;
	
	// Funzioni per il Mouse Hover
	UFUNCTION()
	void OnMouseEnter(UPrimitiveComponent* TouchedComponent);
	// 🔁 CAMBIA questa riga
	virtual void MoveAlongPath(const TArray<AGridTile*>& Path) override;


	UFUNCTION()
	void OnMouseExit(UPrimitiveComponent* TouchedComponent);
private:
	void HighlightMovementRange(bool bHighlight);
	bool bIsSelected = false; // Indica se l'unità è selezionata
	
	
};



