// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

USTRUCT()
struct FTankInput
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Input")
	FVector2D MovementInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Input")
	uint32 bFire1 : 1;
	
	void Sanitize();
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Fire1Input(bool Pressed);

private:
	FVector2D RawMovementInput;


};

UCLASS()
class TANKSZOMBIES_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void PostInitializeComponents() override;

private:
	void MoveForward(float AxisValues);
	void MoveRight(float AxisValues);
	void Fire1Pressed();
	void Fire1Released();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* TankDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* TankSprite;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ATurret> TankTurret;
	class ATurret* Turret;

	uint32 bFired : 1;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank")
	FTankInput TankInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed;
};
