#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"

#include "PlayerCharacter.h"
#include "Engine/TimerHandle.h"
#include "Sound/SoundBase.h"


#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDiedDelegate);

UCLASS()
class GUNSURVIVORS_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* EnemyFlipbook;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	APlayerCharacter* Player;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAlive = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanFollow = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistance = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* DeadFP;

	FTimerHandle DestroyTimer;

	FEnemyDiedDelegate EnemyDiedDelegate;

	void Die();
	void OnDestroyTimerTimeout();
};
