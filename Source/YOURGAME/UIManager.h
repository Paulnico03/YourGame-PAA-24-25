//UIManager.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIManager.generated.h"

class UButton;

UCLASS()
class YOURGAME_API UUIManager : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UButton* BrawlerButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SniperButton;

	UFUNCTION()
	void OnBrawlerSelected();

	UFUNCTION()
	void OnSniperSelected();
};
