// Fill out your copyright notice in the Description page of Project Settings.

#include "TanksZombies.h"
#include "Zombie.h"
#include "Tank.h"



// Sets default values
AZombie::AZombie()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ZombieAI(DeltaTime);

	ConsumeMovementInputVector();
	ConsumeRotationInput();
	ConsumeAttackInput();
}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

void AZombie::SetTarget(AActor* NewTarget)
{
	Target = NewTarget;
	TargetAsTank = Cast<ATank>(NewTarget);
}

AActor* AZombie::GetTarget()
{
	return Target;
}

ATank* AZombie::GetTargetAsTank()
{
	return TargetAsTank;
}

void AZombie::ZombieAI_Implementation(float DeltaTime)
{
	// First we calculate zombie movement and new zombie location - only if zombie can not attack
	FVector MovementInput = (FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(WalkSpeed, RunSpeed), GetPendingMovementInputVector().X)) * DeltaTime * GetActorForwardVector();
	if (GetAttackInput())
	{
		UE_LOG(LogTemp, Warning, TEXT("ATTTTTAAAACK!"));
	}
	FVector DesiredMovement = GetAttackInput() ? FVector::ZeroVector : MovementInput;
	FVector OriginalLocation = GetActorLocation();
	FVector DesiredLocation = OriginalLocation + DesiredMovement;
	// Setting zombie rotation
	float MaxYawThisFrame = YawSpeed * DeltaTime;
	FRotator DesiredRotation = GetActorRotation() + FRotator(0.0f, FMath::Clamp(GetRotationInput(), -MaxYawThisFrame, MaxYawThisFrame), 0.0f);

	SetActorLocationAndRotation(DesiredLocation, DesiredRotation.Quaternion(), true);

	// Checks if we should play zombie walking animation
	FVector OurNewLocation = GetActorLocation();
	FVector DistanceWalked = OurNewLocation - OriginalLocation;
	if (!DistanceWalked.IsNearlyZero())
	{
		ZombieWalk(DeltaTime, DistanceWalked);
	}

	// Attack part
	if (AActor* Target = GetTarget())
	{
		// SHOULD CHECK ATTACK COOLDOWN!!
		if (GetAttackInput())
		{
			FVector DirectionToTarget = (Target->GetActorLocation() - OurNewLocation).GetSafeNormal2D();
			float DotToTarget = FVector::DotProduct(DirectionToTarget, GetActorForwardVector());
			if (DotToTarget >= FMath::Cos(FMath::DegreesToRadians(AttackAngle)))
			{
				float DistSquaredXY = FVector::DistSquaredXY(Target->GetActorLocation(), OurNewLocation);
				if (DistSquaredXY <= (AttackDistance * AttackDistance))
				{
					// CAN  ATTACK
				}
			}
		}
	}
	// Looking for targets
	else
	{
		Target = UGameplayStatics::GetPlayerPawn(this, 0);
		float DistSqXY = FVector::DistSquaredXY(Target->GetActorLocation(), OurNewLocation);
		if (DistSqXY <= (SightDistance * SightDistance))
		{
			FVector DirectionToTarget = (Target->GetActorLocation() - OurNewLocation).GetSafeNormal2D();
			if (FVector::DotProduct(DirectionToTarget, GetActorForwardVector()) >= FMath::Cos(FMath::DegreesToRadians(SightAngle)))
			{
				UE_LOG(LogTemp, Warning, TEXT("Target acuqired"));
				SetTarget(Target);
			}
		}
	}
}

bool AZombie::ZombieAIShouldAttack_Implementation()
{
	if (AActor* Target = GetTarget())
	{
		FVector OurLocation = GetActorLocation();
		FVector DirectionToTarget = (Target->GetActorLocation() - OurLocation).GetSafeNormal2D();
		float DotToTarget = FVector::DotProduct(DirectionToTarget, OurLocation.GetSafeNormal2D());
		UE_LOG(LogTemp, Warning, TEXT("Dot to target = %f"), DotToTarget);
		UE_LOG(LogTemp, Warning, TEXT("Cos = %f"), FMath::Cos(FMath::DegreesToRadians(DotToTarget)));
		if (DotToTarget >= FMath::Cos(FMath::DegreesToRadians(DotToTarget)))
		{
			UE_LOG(LogTemp, Warning, TEXT("Distance = %f"), DirectionToTarget.Size2D());
			if (DirectionToTarget.Size2D() <= (AttackDistance * AttackDistance))
			{
				return true;
			}
		}
	}
	return false;
}

void AZombie::AddRotationInput(float DeltaYawDegrees)
{
	YawInput += DeltaYawDegrees;
}

float AZombie::GetRotationInput()
{
	return YawInput;
}

float AZombie::ConsumeRotationInput()
{
	float RetVal = YawInput;
	YawInput = 0.0f;
	return RetVal;
}

void AZombie::AddAttackInput()
{
	bAttackInput = true;
}

bool AZombie::GetAttackInput()
{
	return bAttackInput;
}

bool AZombie::ConsumeAttackInput()
{
	bool bRetVal = bAttackInput;
	bAttackInput = false;
	return bRetVal;
}

void AZombie::ReceiveDamage(int32 IncomingDamage)
{
	if (IncomingDamage > 0)
	{
		Health -= IncomingDamage;
		if (Health <= 0)
		{
			Destroy();
		}
	}
}

float AZombie::GetHealthRemaining()
{
	return Health;
}