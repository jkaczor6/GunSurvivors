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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnemyInterval = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DifficultyDecreaseNumber = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTimeMinimumLimit = 0.5f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int EnemiesSpawned = 0;


	FTimerHandle SpawnTimer;

	void OnSpawnTimerTimeout();
	void StartSpawning();
	void StopSpawning();
	void SpawnEnemy();
};
