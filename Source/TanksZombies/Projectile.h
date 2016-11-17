// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANKSZOMBIES_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Speed of projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Projectile")
	FName MovementCollisionProfile;
	
	void SetDistanceToTarget(FVector& AimPoint);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* ProjectileSprite;

	// Target location
	FVector Destination;
	float TraveledDistance;

	FTimerHandle TimerHandle_LifeTime;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float MaximumDistance;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void OnExplode();
	virtual void OnExplode_Implementation();
	
	void Explode();
};
