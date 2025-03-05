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

	virtual void PerformAction() override;
};
