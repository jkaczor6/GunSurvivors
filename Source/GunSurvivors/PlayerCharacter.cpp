#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	Flipbook->SetupAttachment(CapsuleComp);

	GunParent = CreateDefaultSubobject<USceneComponent>(TEXT("GunParent"));
	GunParent->SetupAttachment(CapsuleComp);

	GunSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("GunSprite"));
	GunSprite->SetupAttachment(GunParent);

	BulletSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPosition"));
	BulletSpawnPosition->SetupAttachment(GunSprite);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* Player = Cast<APlayerController>(Controller);
	if (Player)
	{
		Player->SetShowMouseCursor(true);

		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(IMC, 0);
		}
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanMove)
	{
		if (MovementDirection.Length() > 0.0f)
		{
			if (MovementDirection.Length() > 1.0f)
			{
				MovementDirection.Normalize();
			}
			FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;
			FVector CurrentLocation = GetActorLocation();
			FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.0f, 0.0f);
			if (!IsInMapBoundsHorizontal(NewLocation.X))
			{
				NewLocation -= FVector(DistanceToMove.X, 0.0f, 0.0f);
			}

			NewLocation += FVector(0.0f, 0.0f, DistanceToMove.Y);
			if (!IsInMapBoundsVertical(NewLocation.Z))
			{
				NewLocation -= FVector(0.0f, 0.0f, DistanceToMove.Y);
			}
			SetActorLocation(NewLocation);
		}
	}

	APlayerController* Player = Cast<APlayerController>(Controller);
	if (Player)
	{
		FVector MouseWorldLocation, MouseWorldDirection;
		Player->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		FVector Start = FVector(GetActorLocation().X, 0.0f, GetActorLocation().Z);
		FVector Target = FVector(MouseWorldLocation.X, 0.0f, MouseWorldLocation.Z);

		FRotator GunParentRotator = UKismetMathLibrary::FindLookAtRotation(Start, Target);

		GunParent->SetRelativeRotation(GunParentRotator);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this , &APlayerCharacter::MoveTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this , &APlayerCharacter::MoveCompleted);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this , &APlayerCharacter::MoveCompleted);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this , &APlayerCharacter::Shoot);
	}
}

void APlayerCharacter::MoveTriggered(const FInputActionValue& Value)
{
	FVector2D MoveActionValue = Value.Get<FVector2D>();

	if (CanMove)
	{
		MovementDirection = MoveActionValue;
		Flipbook->SetFlipbook(Run);

		FVector FlipbookScale = Flipbook->GetComponentScale();
		if (MovementDirection.X < 0.0f && FlipbookScale.X > 0.0f)
		{
			Flipbook->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
		}
		else if (MovementDirection.X > 0.0f && FlipbookScale.X < 0.0f)
		{
			Flipbook->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
		}
	}
}

void APlayerCharacter::MoveCompleted(const FInputActionValue& Value)
{
	MovementDirection = FVector2D::Zero();
	Flipbook->SetFlipbook(Idle);
}

void APlayerCharacter::Shoot(const FInputActionValue& Value)
{
	if (CanShoot)
	{
		CanShoot = false;

		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletActorToSpawn, BulletSpawnPosition->GetComponentLocation(), FRotator(0.0f, 0.0f, 0.0f));
		if (Bullet)
		{
			APlayerController* Player = Cast<APlayerController>(Controller);
			if (Player)
			{
				FVector MouseWorldLocation, MouseWorldDirection;
				Player->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

				FVector CurrentLocation = GetActorLocation();
				FVector2D BulletDirection = FVector2D(MouseWorldLocation.X - CurrentLocation.X, MouseWorldLocation.Z - CurrentLocation.Z);
				BulletDirection.Normalize();

				Bullet->Launch(BulletDirection, BulletSpeed);
			}
		}

		GetWorldTimerManager().SetTimer(ShootCooldownTimer, this, &APlayerCharacter::OnShootCooldownTimerTimeout, 1.0f, false, ShootCooldownDurationInSeconds);
	}
}

bool APlayerCharacter::IsInMapBoundsHorizontal(float XPos)
{
	bool Result = true;

	Result =  (XPos > HorizontalLimits.X) && (XPos < HorizontalLimits.Y);

	return Result;
}

bool APlayerCharacter::IsInMapBoundsVertical(float ZPos)
{
	bool Result = true;

	Result = (ZPos > VerticalLimits.X) && (ZPos < VerticalLimits.Y);

	return Result;
}

void APlayerCharacter::OnShootCooldownTimerTimeout()
{
	CanShoot = true;
}

