// Fill out your copyright notice in the Description page of Project Settings.

#include "TanksZombies.h"
#include "PaperSpriteComponent.h"
#include "Tank.h"
#include "Turret.h"


void FTankInput::Sanitize()
{
	MovementInput = RawMovementInput.ClampAxes(-1.0f, 1.0f);
	// если движение по диагонали, ограничим скорость
	MovementInput = MovementInput.GetSafeNormal();
	RawMovementInput.Set(0.0f, 0.0f);
}

void FTankInput::MoveForward(float AxisValues)
{
	RawMovementInput.X += AxisValues;
}

void FTankInput::MoveRight(float AxisValues)
{
	RawMovementInput.Y += AxisValues;
}

void FTankInput::Fire1Input(bool bPressed)
{
	bFire1 = bPressed;
}


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TankBase"));
	}

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	TankDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("TankDirection"));
	TankDirection->SetupAttachment(RootComponent);

	TankSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TankSprite"));
	TankSprite->SetupAttachment(TankDirection);

	//TankTurret = CreateDefaultSubobject<UChildActorComponent>(TEXT("TankTurret"));
	//TankTurret->SetupAttachment(TankDirection);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->CameraLagSpeed = 2.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	CameraComponent->OrthoWidth = 1024.0f;
	CameraComponent->AspectRatio = 4.0f / 3.0f;
	CameraComponent->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));

	MovementSpeed = 100.0f;
	RotationSpeed = 180.0f;

}

void ATank::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Turret = GetWorld()->SpawnActor<ATurret>(TankTurret);

	if (Turret)
	{
		Turret->SetActorLocation(FVector(0.0f, 0.0f, 1.0f));
		Turret->AttachToComponent(TankDirection, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATank::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	TankInput.Sanitize();

	FVector DesiredMovementDirection = FVector(TankInput.MovementInput.X, TankInput.MovementInput.Y, 0.0f);
	if (!DesiredMovementDirection.IsNearlyZero())
	{
		// HACK: танк слишком быстро поворачивается, если поворачивается на месте (без движения вперед\назад)
		if (TankInput.MovementInput.X == 0.0f)
		{
			TankDirection->AddLocalRotation(FRotator(0.0f, DesiredMovementDirection.Y * RotationSpeed * DeltaTime/2, 0.0f));
		}
		else
		{
			TankDirection->AddLocalRotation(FRotator(0.0f, DesiredMovementDirection.Y * RotationSpeed * DeltaTime, 0.0f));
		}	

		if (TankInput.MovementInput.X != 0.0f)
		{
			FVector forward = FMath::Sign(TankInput.MovementInput.X) * TankDirection->GetForwardVector();

			FVector pos = GetActorLocation();
			pos.X += MovementSpeed * forward.X * DeltaTime;
			pos.Y += MovementSpeed * forward.Y * DeltaTime;

			SetActorLocation(pos);
		}
	}

	if (Turret && TankInput.bFire1 && !bFired)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire event! TankInput.bFire1 = %d , bFired = %d"), TankInput.bFire1, bFired);
		Turret->Fire();
	}

	
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ATank::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATank::MoveRight);
	InputComponent->BindAction("Fire1", EInputEvent::IE_Pressed, this, &ATank::Fire1Pressed);
	InputComponent->BindAction("Fire1", EInputEvent::IE_Released, this, &ATank::Fire1Released);

}

void ATank::MoveForward(float AxisValues)
{
	TankInput.MoveForward(AxisValues);
}

void ATank::MoveRight(float AxisValues)
{
	TankInput.MoveRight(AxisValues);
}

void ATank::Fire1Pressed()
{
	TankInput.Fire1Input(true);
}

void ATank::Fire1Released()
{
	TankInput.Fire1Input(false);
}