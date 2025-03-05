#pragma once

#include "CoreMinimal.h"
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

    UPROPERTY(EditAnywhere, Category = "Grid")
    bool bIsObstacle = false; 

    void SetGridPosition(int32 X, int32 Y);
    void SetAsObstacle();
    void SetAsNormal();
};
