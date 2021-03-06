// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VecOps.generated.h"

/*
		Contains operations related to R3 vectors
*/

UCLASS()
class MESHTECH_API UVecOps : public UObject
{
	GENERATED_BODY()
public:

	/*
			NPointEnclosure2D

			Returns an array N points enclosing a center point evenly spaced using 360.0 / N degrees unit vectors 
			of length R in plane defined by Normal rotated around Center
	*/
	static TArray<FVector> NPointEnclosure2D(FVector Center, float R, FVector Normal, int32 N, FVector GuideLine[2]);

	/*
			MaxComponentDir

			Return a unit vector with the max component of the original set to 1.0 and all
			other components set to 0.0
	*/
	static FVector MaxComponentDir(FVector V);

	/*
			GetOrthogonal

			Returns an arbitrary(?) unit vector in the plane orthogonal to V
	*/
	static FVector GetOrthogonal(FVector V);

	/*
			GetBezierPoint (Quadratic)

			Gets a quadratic Bezier point at T between P0 and P2 using control point P1
	*/
	static FVector GetBezierPoint(float T, FVector P0, FVector P1, FVector P2);

	/*
			GetBezierPoint (Cubic)

			Gets a cubic Bezier point at T between P0 and P3 using control points P1 and P2
	*/
	static FVector GetBezierPoint(float T, FVector P0, FVector P1, FVector P2, FVector P3);

};
