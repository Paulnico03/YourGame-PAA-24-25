//UnitBase.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitBase.generated.h"

UCLASS()
class YOURGAME_API AUnitBase : public AActor
{
	GENERATED_BODY()

public:
	AUnitBase();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	FString UnitName;

	UFUNCTION(BlueprintCallable, Category = "Unit")
	virtual void PerformAction();

};
