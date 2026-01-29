#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "Engine/TimerHandle.h"

#include "Bullet.generated.h"

UCLASS()
class GUNSURVIVORS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* BulletSprite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 300.0f;
	UPROPERTY(BlueprintReadWrite)
	FVector2D MovementDirection;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsLaunched = false;

	FTimerHandle DeleteTimer;

	void Launch(FVector2D Direction, float Speed);
	void OnDeleteTimerTimeout();
};
