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
    ATopDownGameMode* GameMode = Cast<ATopDownGameMode>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    if (GameMode->bBrawlerPlaced) // NON selected
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ Brawler già posizionato, non puoi sceglierlo di nuovo!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("🔥 Brawler Button Clicked!"));
    GameMode->SelectUnit(EUnitType::Brawler);
}

void UUIManager::OnSniperSelected()
{
    ATopDownGameMode* GameMode = Cast<ATopDownGameMode>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    if (GameMode->bSniperPlaced)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ Sniper già posizionato, non puoi sceglierlo di nuovo!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("🔥 Sniper Button Clicked!"));
    GameMode->SelectUnit(EUnitType::Sniper);
}


void UUIManager::DisableUnitSelection()
{
    if (BrawlerButton)
    {
        BrawlerButton->SetIsEnabled(false); // Disabilita il pulsante del Brawler
        UE_LOG(LogTemp, Warning, TEXT("🚫 BrawlerButton disabilitato!"));
    }

    if (SniperButton)
    {
        SniperButton->SetIsEnabled(false); // Disabilita il pulsante dello Sniper
        UE_LOG(LogTemp, Warning, TEXT("🚫 SniperButton disabilitato!"));
    }
}
