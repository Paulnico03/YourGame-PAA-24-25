//UIManager.cpp
#include "UIManager.h"
#include "Components/Button.h"
#include "TopDownGameMode.h"

void UUIManager::NativeConstruct()
{
    Super::NativeConstruct();

    UE_LOG(LogTemp, Warning, TEXT("🔥 UIManager::NativeConstruct() chiamato!"));

    if (BrawlerButton)
    {
        BrawlerButton->OnClicked.AddDynamic(this, &UUIManager::OnBrawlerSelected);
        UE_LOG(LogTemp, Warning, TEXT("✅ BrawlerButton binding corretto!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ BrawlerButton è NULL!"));
    }

    if (SniperButton)
    {
        SniperButton->OnClicked.AddDynamic(this, &UUIManager::OnSniperSelected);
        UE_LOG(LogTemp, Warning, TEXT("✅ SniperButton binding corretto!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ SniperButton è NULL!"));
    }
}

void UUIManager::OnBrawlerSelected()
{
    UE_LOG(LogTemp, Warning, TEXT("🔥 Brawler Button Clicked!"));

    ATopDownGameMode* GameMode = Cast<ATopDownGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->SelectUnit(EUnitType::Brawler);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GameMode non trovato!"));
    }
}

void UUIManager::OnSniperSelected()
{
    UE_LOG(LogTemp, Warning, TEXT("🔥 Sniper Button Clicked!"));

    ATopDownGameMode* GameMode = Cast<ATopDownGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->SelectUnit(EUnitType::Sniper);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GameMode non trovato!"));
    }
}
