// Fill out your copyright notice in the Description page of Project Settings.

#include "TanksZombies.h"
#include "UMath2DHelpers.h"



bool UMath2DHelpers::FindLookAtAngle2D(const FVector2D& Start, const FVector2D& End, float& Angle)
{
	FVector2D direction = (End - Start).GetSafeNormal();
	if (!direction.IsNearlyZero())
	{
		Angle = FMath::RadiansToDegrees(FMath::Atan2(direction.Y, direction.X));
		return true;
	}
	return false;
}

float UMath2DHelpers::FindDeltaAngelDegrees(float A1, float A2)
{
	float delta = A2 - A1;
	if (delta > 180.0f)
	{
		delta -= 360.0f;
	}
	else if (delta < -180.0f)
	{
		delta += 360.0f;
	}
	return delta;
}