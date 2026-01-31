#include "Enemy.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	EnemyFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemyFlipbook"));
	EnemyFlipbook->SetupAttachment(CapsuleComp);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlive && CanFollow && Player)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector PlayerLocation = Player->GetActorLocation();

		FVector DirectionToPlayer = PlayerLocation - CurrentLocation;
		float DistanceToPlayer = DirectionToPlayer.Length();
		
		if (DistanceToPlayer >= StopDistance)
		{
			DirectionToPlayer.Normalize();

			FVector NewLocation = CurrentLocation + DirectionToPlayer * MovementSpeed * DeltaTime;
			SetActorLocation(NewLocation);
		}

		CurrentLocation = GetActorLocation();
		float FlipbookXScale = EnemyFlipbook->GetComponentScale().X;

		if ((PlayerLocation.X - CurrentLocation.X) >= 0.0f)
		{
			if(FlipbookXScale < 0.0f) EnemyFlipbook->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
			
		}
		else
		{
			if (FlipbookXScale > 0.0f) EnemyFlipbook->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
		}
	}
}

void AEnemy::Die()
{
	if (!IsAlive) return;

	IsAlive = false;
	CanFollow = false;
	EnemyFlipbook->SetFlipbook(DeadFP);
	EnemyFlipbook->SetTranslucentSortPriority(-2);

	EnemyDiedDelegate.Broadcast();

	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AEnemy::OnDestroyTimerTimeout, 1.0f, false, 10.0f);
}

void AEnemy::OnDestroyTimerTimeout()
{
	Destroy();
}

