// Fill out your copyright notice in the Description page of Project Settings.

#include "BeamGen.h"

void UBeamGen::GenerateBeam(FVector2D BeamDim, FVector P1, FVector P2, FRotator R, UProceduralMeshComponent* Promesh, int32 Index)
{
	//Promesh->ClearAllMeshSections();
	FVector Dir;
	float Len;
	(P2 - P1).ToDirectionAndLength(Dir, Len);

	TArray<FVector> Q1 = GetBeamQuad(BeamDim, P1, Dir.ToOrientationRotator());
	TArray<FVector> Q2 = GetBeamQuad(BeamDim, P2, Dir.ToOrientationRotator());

	Q1.Append(Q2);
	UPointConnector::Connect8P(Q1, Promesh, Index);
}

TArray<FVector> UBeamGen::GetBeamQuad(FVector2D BeamDim, FVector QuadCenter, FRotator R)
{
	FVector P00, P10, P01, P11;
	P00 = QuadCenter;
	P10 = QuadCenter;
	P01 = QuadCenter;
	P11 = QuadCenter;

	FVector P00Offset = { 0, -BeamDim.X / 2, -BeamDim.Y / 2 };
	FVector P10Offset = { 0, BeamDim.X / 2, -BeamDim.Y / 2 };
	FVector P01Offset = { 0, -BeamDim.X / 2, BeamDim.Y / 2 };
	FVector P11Offset = { 0, BeamDim.X / 2, BeamDim.Y / 2};

	P00 += R.RotateVector(P00Offset);
	P10 += R.RotateVector(P10Offset);
	P01 += R.RotateVector(P01Offset);
	P11 += R.RotateVector(P11Offset);

	return { P00, P10, P01, P11 };
}