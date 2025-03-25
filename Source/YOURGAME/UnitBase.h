//UnitBase.h
#pragma once

#include "GridTile.h"
#include "TimerManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitBase.generated.h"



UCLASS()
class YOURGAME_API AUnitBase : public AActor
{
	GENERATED_BODY()
	// All'interno della classe AUnitBase
public:
	void StartMovingAlongPath(const TArray<AGridTile*>& Path);

protected:
	void MoveStep();

private:
	UPROPERTY()
	TArray<AGridTile*> PathToFollow;

	int32 CurrentPathIndex;

	FTimerHandle MovementTimerHandle;
public:
	AUnitBase();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 MovementRange = 3; // o il valore che preferisci

	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	FString UnitName;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 GetMovementRange() const { return MovementRange; }

	UFUNCTION(BlueprintCallable, Category = "Unit")
	virtual void PerformAction();
	virtual void MoveAlongPath(const TArray<AGridTile*>& Path);

	UPROPERTY()
	TArray<AGridTile*> Path;

	UPROPERTY()
	AGridTile* CurrentTile;

};
