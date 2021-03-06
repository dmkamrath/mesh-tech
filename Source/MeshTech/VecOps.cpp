// Fill out your copyright notice in the Description page of Project Settings.


#include "VecOps.h"

/*
		NPointEnclosure2D
*/
TArray<FVector> UVecOps::NPointEnclosure2D(FVector Center, float R, FVector Normal, int32 N, FVector GuideLine[2])
{
	TArray<FVector> Pts;
	Normal.Normalize();
	FVector Ortho = GetOrthogonal(Normal);

	if (!GuideLine[0].IsZero())
		Ortho = (FMath::LinePlaneIntersection(GuideLine[0], GuideLine[1], Center, Normal) - Center).GetSafeNormal();

	for (int32 I = 0; I < N; I++)
		Pts.Add(Center + (Ortho.RotateAngleAxis((360.0 / N) * I, Normal) * R));
	
	return Pts;
}

/*
		MaxComponentDir
*/
FVector UVecOps::MaxComponentDir(FVector V)
{
	float MaxComponent = V.GetAbsMax();
	for (int32 I = 0; I < 3; I++)
		V[I] = FMath::Abs(V[I]) == MaxComponent ? 1.0 : 0.0;
	return V;
}

/*
		GetOrthogonal
*/
FVector UVecOps::GetOrthogonal(FVector V)
{
	FVector Ortho(0, 0, 0);
	for (int32 I = 0; I < 3; I++)
	{
		if (V[I] != 0)
		{
			int32 N = (I + 1) % 3;
			Ortho[N] = V[I];
			Ortho[I] = -V[N];
			return Ortho.GetSafeNormal();
		}
	}
	return FVector(0, 0, 0);
}

/*
		GetBezierPoint (Quadratic)
*/
FVector UVecOps::GetBezierPoint(float T, FVector P0, FVector P1, FVector P2)
{
	return (1 - T) * ((1 - T) * P0 + T * P1) + T * ((1 - T) * P1 + T * P2);
}

/*
		GetBezierPoint (Cubic)
*/
FVector UVecOps::GetBezierPoint(float T, FVector P0, FVector P1, FVector P2, FVector P3)
{
	float Z = 1 - T;
	float Zsq = Z * Z;
	return Z * Zsq * P0 + 3.0 * Zsq * T * P1 + 3.0 * Z * T * T * P2 + T * T * T * P3;
}