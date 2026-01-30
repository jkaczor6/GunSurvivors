#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"

#include "Engine/TimerHandle.h"
#include "Bullet.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class GUNSURVIVORS_API APlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* Flipbook;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* GunParent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* GunSprite;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* BulletSpawnPosition;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* IMC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* ShootAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 100.0f;
	UPROPERTY(BlueprintReadWrite)
	FVector2D MovementDirection;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanMove = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook* Idle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook* Run;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D HorizontalLimits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D VerticalLimits;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanShoot = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootCooldownDurationInSeconds = 0.3f;

	FTimerHandle ShootCooldownTimer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABullet> BulletActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletSpeed = 300.0f;

	void MoveTriggered(const FInputActionValue& Value);
	void MoveCompleted(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	
	bool IsInMapBoundsHorizontal(float XPos);
	bool IsInMapBoundsVertical(float ZPos);
	void OnShootCooldownTimerTimeout();
};