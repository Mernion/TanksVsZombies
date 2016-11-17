// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "DamageInterface.h"
#include "Zombie.generated.h"


UCLASS()
class TANKSZOMBIES_API AZombie : public APawn, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AZombie();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Zombie health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie")
	float Health;

	// Zombie walk speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie")
	float WalkSpeed;

	// Zombie run speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie")
	float RunSpeed;

	// Zombie rotation speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie")
	float YawSpeed;

	// Zombie distance sight - how far away zombie can spot target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie")
	float SightDistance;

	// Zombie sight angle - specifies view area in which zombie can spot targets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie")
	float SightAngle;

	// Zombie attack rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie")
	float AttackCooldoown;

	// How much damage zombie can do in one hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie")
	float AttackDamage;

	// Distance at which zombie can attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie")
	float AttackDistance;

	// Creates maximum rotation offset for attacking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie")
	float AttackAngle;

	// ZOMBIE FOOD
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTarget(AActor* NewTarget);

	// WHO IS ZOMBIE FOOD TODAY
	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTarget();

	// ZOMBIE FOOD AS TANK
	UFUNCTION(BlueprintCallable, Category = "AI")
	class ATank* GetTargetAsTank();

	// Called instead of tick function - should be called by AI controller
	UFUNCTION(BlueprintNativeEvent, Category = "AI")
	void ZombieAI(float DeltaTime);
	virtual void ZombieAI_Implementation(float DeltaTime);

	// Called to check if Zombie can eat-eat or stay hungry :(
	UFUNCTION(BlueprintNativeEvent, Category = "AI")
	bool ZombieAIShouldAttack();
	virtual bool ZombieAIShouldAttack_Implementation();

	// Zombie walk animation should be handled here
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void ZombieWalk(float DeltaTime, FVector DistanceWalkedThisFrame);

	// Zombie attack animation should be handled here
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void ZombieAttack(float DeltaTime);
	
	// ZOMBIE INPUT //

	// Add yaw (in degrees) to zombie rotation
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input")
	virtual void AddRotationInput(float DeltaYawDegrees);

	// Get rotation input
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input")
	virtual float GetRotationInput();

	UFUNCTION(BlueprintCallable, Category = "Pawn|Input")
	virtual float ConsumeRotationInput();

	UFUNCTION(BlueprintCallable, Category = "Pawn|Input")
	virtual void AddAttackInput();

	UFUNCTION(BlueprintCallable, Category = "Pawn|Input")
	virtual bool GetAttackInput();

	UFUNCTION(BlueprintCallable, Category = "Pawn|Input")
	virtual bool ConsumeAttackInput();

	// DAMAGE INPUT
	virtual void ReceiveDamage(int32 IncomingDamage) override;
	virtual float GetHealthRemaining() override;

private:
	uint32 bAttackInput : 1;

	FTimerHandle TimerHandle_AttackCooldown;

	float YawInput;

	UPROPERTY(VisibleInstanceOnly, Category = "AI")
	AActor*	Target;

	UPROPERTY(VisibleInstanceOnly, Category = "AI")
	class ATank* TargetAsTank;

	

};
