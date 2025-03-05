//UnitBase.cpp
#include "UnitBase.h"

AUnitBase::AUnitBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUnitBase::BeginPlay()
{
	Super::BeginPlay();
}

void AUnitBase::PerformAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Azione eseguita da %s"), *UnitName);
}


