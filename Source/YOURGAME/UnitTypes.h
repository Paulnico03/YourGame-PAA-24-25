#pragma once

#include "CoreMinimal.h"
#include "UnitTypes.generated.h"

UENUM(BlueprintType)
enum class EUnitType : uint8
{
	None UMETA(DisplayName = "Nessuna Selezione"),  // 🔹 Aggiunto None per indicare nessuna unità selezionata
	Brawler UMETA(DisplayName = "Brawler"),
	Sniper UMETA(DisplayName = "Sniper")
};