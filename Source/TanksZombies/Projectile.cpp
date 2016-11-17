// Fill out your copyright notice in the Description page of Project Settings.

#include "TanksZombies.h"
#include "Projectile.h"
#include "PaperSpriteComponent.h"
#include "DamageInterface.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ProjectileSprite"));

	Speed = 100.0f;
	MaximumDistance = 800.0f;
	TraveledDistance = 0.0f;
	Radius = 20.0f;
	Damage = 10.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_LifeTime, this, &AProjectile::Explode, 10.0f);
}

void AProjectile::SetDistanceToTarget(FVector& AimPoint)
{
	Destination = AimPoint;
}

// Called every frame
void AProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector Loc = GetActorLocation();
	float Distance = (Destination - Loc).Size();
	if (Distance <= 0.5f || TraveledDistance >= MaximumDistance)
	{
		Explode();
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Distance to AimPoint = %f"), Distance);
		FVector DesiredLoc = Loc + (Speed * DeltaTime) * GetActorForwardVector();
		TraveledDistance += Speed * DeltaTime;

		if (UWorld* World = GetWorld())
		{
			FHitResult OutHit;
			FCollisionShape CollisionShape;
			CollisionShape.SetCapsule(Radius, 300.0f);
			if (World->SweepSingleByProfile(OutHit, Loc, DesiredLoc, FQuat::Identity, MovementCollisionProfile, CollisionShape))
			{
				SetActorLocation(OutHit.Location);
				if (IDamageInterface* DamageActor = Cast<IDamageInterface>(OutHit.Actor.Get()))
				{
					DamageActor->ReceiveDamage(Damage);
				}
				Explode();
			}
			else
			{
				SetActorLocation(DesiredLoc);
			}
		}	
	}
}

void AProjectile::Explode()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_LifeTime);
	SetActorEnableCollision(false);
	OnExplode();
}

void AProjectile::OnExplode_Implementation()
{
	Destroy();
}

