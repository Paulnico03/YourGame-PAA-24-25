//TopDownCameraPawn.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TopDownCameraPawn.generated.h"

UCLASS()
class YOURGAME_API ATopDownCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ATopDownCameraPawn();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;
};
