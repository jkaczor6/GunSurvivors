#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Enemy.h"
#include "Engine/TimerHandle.h"

#include "EnemySpawner.generated.h"

UCLASS()
class GUNSURVIVORS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> Enemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spawnTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spawnDistance = 400.0f;

	FTimerHandle SpawnTimer;

	void OnSpawnTimerTimeout();
	void StartSpawning();
	void StopSpawning();
	void SpawnEnemy();
};
