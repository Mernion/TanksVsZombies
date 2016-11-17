// Fill out your copyright notice in the Description page of Project Settings.

#include "TanksZombies.h"
#include "ZombieBrains.h"
#include "Zombie.h"

void AZombieBrains::Tick(float DeltaTime)
{
	if (PawnAsZombie)
	{
		if (AActor* Target = PawnAsZombie->GetTarget())
		{
			//UE_LOG(LogTemp, Warning, TEXT("Going to target"));
			FVector DirectionToTarget = (Target->GetActorLocation() - PawnAsZombie->GetActorLocation()).GetSafeNormal2D();
			float DotToTarget = FVector::DotProduct(DirectionToTarget, PawnAsZombie->GetActorForwardVector());
			float SidewaysDotToTarget = FVector::DotProduct(DirectionToTarget, PawnAsZombie->GetActorRightVector());
			float DeltaYawDesired = FMath::Atan2(SidewaysDotToTarget, DotToTarget);

			if (PawnAsZombie->ZombieAIShouldAttack())
			{
				PawnAsZombie->AddAttackInput();
			}
			else
			{
				PawnAsZombie->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), FMath::GetMappedRangeValueClamped(FVector2D(-0.707f, 0.707f), FVector2D(0.0f, 1.0f), DotToTarget));
				PawnAsZombie->AddRotationInput(DeltaYawDesired);
			}

		}
	}
}

void AZombieBrains::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	PawnAsZombie = Cast<AZombie>(GetPawn());
}

void AZombieBrains::UnPossess()
{
	Super::UnPossess();
	PawnAsZombie = nullptr;
}


