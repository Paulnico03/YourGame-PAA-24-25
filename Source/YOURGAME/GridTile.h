//GridTile.h
#pragma once

#include "CoreMinimal.h"
#include "UnitTypes.h"
#include "GameFramework/Actor.h"
#include "GridTile.generated.h"

UCLASS()
class YOURGAME_API AGridTile : public AActor
{
    GENERATED_BODY()

public:
    AGridTile();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 XIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 YIndex;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* TileMesh;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* ObstacleMesh; 

    UPROPERTY(EditAnywhere, Category = "Grid")
    UMaterialInterface* GridMaterial_Normal; 

    UPROPERTY(EditAnywhere, Category = "Obstacle")
    UMaterialInterface* ObstacleMaterial1; 

    UPROPERTY(EditAnywhere, Category = "Obstacle")
    UMaterialInterface* ObstacleMaterial2; 

    UPROPERTY(EditAnywhere, Category = "Grid")
    UMaterialInterface* GridMaterial;
    // Materiali delle unità
    UPROPERTY(EditAnywhere, Category = "Materials")
    UMaterialInterface* BrawlerMaterial;

    UPROPERTY(EditAnywhere, Category = "Materials")
    UMaterialInterface* SniperMaterial;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    UMaterialInterface* DefaultMaterial;


    UFUNCTION()
    FVector2D GetGridPosition() const;
    UPROPERTY()
    int GridX;

    void SetHighlighted(bool bHighlight);

    UPROPERTY()
    int GridY;


    // Funzione per cambiare il materiale in base all'unità selezionata
    void SetUnitMaterial(EUnitType UnitType);
    
    virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

    UPROPERTY(EditAnywhere, Category = "Grid")
    UMaterialInterface* HighlightMaterial;

    UPROPERTY(EditAnywhere, Category = "Grid")
    bool bIsObstacle = false;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool bIsHighlighted = false;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool bIsBlocked = false;


    void SetHighlight(bool bHighlight);
    void SetGridPosition(int32 X, int32 Y);
    void SetAsObstacle();
    void SetAsNormal();
private:
    bool bIsOccupied = false; // ✅ Controlla se la cella è già occupata

    
};
