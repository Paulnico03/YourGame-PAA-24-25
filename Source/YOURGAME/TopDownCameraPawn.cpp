
#include "TopDownCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"

#include "TopDownCameraPawn.h"

ATopDownCameraPawn::ATopDownCameraPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	RootComponent = CameraComponent;

	// Imposta la posizione della camera
	FVector CameraPosition = FVector(1312.5f, 400.0f, 2625.0f);
	CameraComponent->SetWorldLocation(CameraPosition);

	// Imposta la rotazione della camera per guardare in basso
	FRotator CameraRotation = FRotator(-90.0f, 0.0f, 0.0f);
	CameraComponent->SetWorldRotation(CameraRotation);
}


void ATopDownCameraPawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->SetViewTarget(this); // Imposta la visuale sulla camera fissa
		PlayerController->bShowMouseCursor = true; // Mostra il cursore
		PlayerController->SetIgnoreMoveInput(true); // Disabilita il movimento del personaggio
		PlayerController->SetIgnoreLookInput(true); // Disabilita la rotazione della visuale
	}
}
