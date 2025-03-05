//UnitTypes.h
#pragma once

#include "CoreMinimal.h"
#include "UnitTypes.generated.h"  // ✅ Necessario per il sistema di Unreal Engine

UENUM(BlueprintType)
enum class EUnitType : uint8
{
	Brawler UMETA(DisplayName = "Brawler"),
	Sniper UMETA(DisplayName = "Sniper")
};
