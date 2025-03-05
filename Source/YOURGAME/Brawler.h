//Brawler.h
#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "Brawler.generated.h"

UCLASS()
class YOURGAME_API ABrawler : public AUnitBase
{
	GENERATED_BODY()

public:
	ABrawler();

	virtual void PerformAction() override;
};
