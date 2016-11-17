// Fill out your copyright notice in the Description page of Project Settings.

#include "TanksZombies.h"
#include "PaperSpriteComponent.h"
#include "Turret.h"
#include "Projectile.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = TurretDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("TurretDirection"));

	TurretSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TurretSprite"));
	TurretSprite->SetupAttachment(TurretDirection);

	RotationSpeed = 180.0f;
	RateOfFire = 1.0f;
	bCanFire = true;
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	Tank = Cast<APawn>(GetAttachParentActor());
	if (Tank)
	{
		PC = Cast<APlayerController>(Tank->GetController());
	}

}

// Called every frame
void ATurret::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (Tank && PC)
	{
		FHitResult Hit;
		if (PC->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), false, Hit))
		{
			FVector Direction = Hit.Location - GetActorLocation();
			FRotator DesiredRot = FRotationMatrix::MakeFromX(Direction).Rotator();
			DesiredRot.Pitch = GetActorRotation().Pitch;
			//UE_LOG(LogTemp, Warning, TEXT("Current rotation: yaw = %f, pitch = %f, roll = %f"), GetActorRotation().Yaw, GetActorRotation().Pitch, GetActorRotation().Roll);
			//UE_LOG(LogTemp, Warning, TEXT("Desired rotation: yaw = %f, pitch = %f, roll = %f"), DesiredRot.Yaw, DesiredRot.Pitch, DesiredRot.Roll);
			FRotator Rotator = FMath::RInterpConstantTo(TurretDirection->GetComponentRotation(), DesiredRot, DeltaTime, RotationSpeed);
			SetActorRotation(Rotator);
		}
	}
}

void ATurret::Fire()
{
	if (Projectile && bCanFire)
	{
		if (UWorld* World = GetWorld())
		{
			FHitResult Hit;
			if (PC->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), false, Hit))
			{
				FVector DistanceToTarget = Hit.Location;
				DistanceToTarget.Z = GetActorLocation().Z;
				if (AProjectile* SpawnedProjectile = World->SpawnActor<AProjectile>(Projectile))
				{
					FVector Loc = TurretSprite->GetSocketLocation(SocketName);
					FRotator Rot = TurretSprite->GetComponentRotation();
					SpawnedProjectile->SetActorLocation(Loc);
					SpawnedProjectile->SetActorRotation(Rot);
					SpawnedProjectile->SetDistanceToTarget(DistanceToTarget);
					//UE_LOG(LogTemp, Warning, TEXT("Spawned projectile!"));
					//UE_LOG(LogTemp, Warning, TEXT("Projectile location: X = %f, Y = %f, Z = %f"), Loc.X, Loc.Y, Loc.Z);
					bCanFire = false;
					//UE_LOG(LogTemp, Warning, TEXT("CanFire = %d"), bCanFire);
					GetWorldTimerManager().SetTimer(TimerHandle_HandleFireRate, this, &ATurret::CheckFireRate, RateOfFire);
				}
			}
		}
	}
}

void ATurret::CheckFireRate()
{
	bCanFire = true;
	UE_LOG(LogTemp, Warning, TEXT("Rate of fire timer is up! bCanFire = %d"), bCanFire);
}

