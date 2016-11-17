// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "ZombieBrains.generated.h"

/**
 * 
 */
UCLASS()
class TANKSZOMBIES_API AZombieBrains : public AAIController
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;

	virtual void Possess(APawn* InPawn) override;
	virtual void UnPossess() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Zombie")
	class AZombie* PawnAsZombie;
	
};
