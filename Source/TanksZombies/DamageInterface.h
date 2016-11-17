// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DamageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class TANKSZOMBIES_API IDamageInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ReceiveDamage(int32 IncomingDamage) = 0;
	virtual float GetHealthRemaining() = 0;
};
