// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Turret.generated.h"

UCLASS()
class TANKSZOMBIES_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void Fire();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	float RateOfFire;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* TurretDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* TurretSprite;

	UPROPERTY(EditDefaultsOnly, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> Projectile;

	UPROPERTY()
	APawn* Tank;

	UPROPERTY()
	APlayerController* PC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	FName SocketName;

	uint32 bCanFire : 1;

	FTimerHandle TimerHandle_HandleFireRate;

	UFUNCTION()
	void CheckFireRate();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed;

	
	
};
