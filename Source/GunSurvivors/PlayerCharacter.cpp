#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	Flipbook->SetupAttachment(CapsuleComp);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* Player = Cast<APlayerController>(Controller);
	if (Player)
	{
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

	if (!CanMove)
	{
		return;
	}
	if (MovementDirection.Length() <= 0.0f)
	{
		return;
	}
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
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Shoot!"));
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

