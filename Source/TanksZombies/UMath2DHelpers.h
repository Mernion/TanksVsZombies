// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UMath2DHelpers.generated.h"

/**
 * 
 */
UCLASS()
class TANKSZOMBIES_API UMath2DHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "math")
	static bool FindLookAtAngle2D(const FVector2D& Start, const FVector2D& End, float& Angle);

	UFUNCTION(BlueprintPure, Category = "math")
	static float FindDeltaAngelDegrees(float A1, float A2);
	
};
