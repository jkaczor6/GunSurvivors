#include "EnemySpawner.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	AGameModeBase* Gamemode = UGameplayStatics::GetGameMode(GetWorld());
	if (Gamemode)
	{
		GM = Cast<AGunSurvivorsGameMode>(Gamemode);
		check(GM);
	}

	AActor* ReturnedActor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	if (ReturnedActor)
	{
		Player = Cast<APlayerCharacter>(ReturnedActor);
		Player->PlayerDiedDelegate.AddDynamic(this, &AEnemySpawner::OnPlayerDied);
	}

	StartSpawning();
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::StartSpawning()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawner::OnSpawnTimerTimeout, spawnTime, true, spawnTime);
}

void AEnemySpawner::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}

void AEnemySpawner::OnSpawnTimerTimeout()
{
	SpawnEnemy();
}

void AEnemySpawner::SpawnEnemy()
{
	FVector2D RandomPosition = FVector2D(FMath::VRand());
	RandomPosition.Normalize();
	RandomPosition *= spawnDistance;

	FVector EnemyLocation = GetActorLocation() + FVector(RandomPosition.X, 0.0f, RandomPosition.Y);

	AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyToSpawn, EnemyLocation, FRotator::ZeroRotator);
	SetupEnemy(SpawnedEnemy);

	EnemiesSpawned++;
	if ((EnemiesSpawned % EnemyInterval) == 0 && spawnTime > SpawnTimeMinimumLimit)
	{
		spawnTime -= DifficultyDecreaseNumber;
		if (spawnTime < SpawnTimeMinimumLimit)
		{
			spawnTime = SpawnTimeMinimumLimit;
		}

		StopSpawning();
		StartSpawning();
	}
}

void AEnemySpawner::SetupEnemy(AEnemy* Enemy)
{
	if (Enemy)
	{
		Enemy->Player = Player;
		Enemy->CanFollow = true;
		Enemy->EnemyDiedDelegate.AddDynamic(this, &AEnemySpawner::OnEnemyDied);
	}
}

void AEnemySpawner::OnEnemyDied()
{
	UGameplayStatics::PlaySound2D(GetWorld(), DeathSound);
	GM->AddScore(ScorePerEnemy);
}

void AEnemySpawner::OnPlayerDied()
{
	StopSpawning();
	UGameplayStatics::PlaySound2D(GetWorld(), DeathSound);

	TArray<AActor*> EnemyArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), EnemyArray);
	for (AActor* EnemyActor : EnemyArray)
	{
		AEnemy* Enemy = Cast<AEnemy>(EnemyActor);
		if (Enemy && Enemy->IsAlive)
		{
			Enemy->CanFollow = false;
		}
	}

	GM->OnRestartGameTimerTimeout();
}
