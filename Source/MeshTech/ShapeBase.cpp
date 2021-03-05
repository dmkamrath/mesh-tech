// Fill out your copyright notice in the Description page of Project Settings.


#include "ShapeBase.h"

void UShapeBase::InitLines()
{
	
}

void UShapeBase::Bend(int32 Divisions, FVector ControlPoint)
{
	for (auto L : Lines)
		L->Bend(Divisions, ControlPoint);
}

void UShapeBase::Bend(int32 Divisions, FVector ControlPoint1, FVector ControlPoint2)
{
	for (auto L : Lines)
		L->Bend(Divisions, ControlPoint1, ControlPoint2);
}

void UCubeBase::InitLines()
{
	FVector X = Dims * FVector::ForwardVector / 2;
	FVector Y = Dims * FVector::RightVector / 2;
	FVector Z = Dims * FVector::UpVector / 2;

	UVertLine* L1 = UVertLine::Ln(Zero, Zero + (Y * Dir));
	UVertLine* L2 = UVertLine::Ln({ Zero + (Z * Dir), Zero + ((Z + Y) * Dir), Zero + (Y * Dir) });
	UVertLine* L3 = UVertLine::Ln({ Zero + ((Z + X) * Dir), Zero + ((Z + X + Y) * Dir), Zero + ((X + Y) * Dir) });
	UVertLine* L4 = UVertLine::Ln(Zero + (X * Dir), Zero + ((X + Y) * Dir));

	// Add lines between L2,L3 pair
	TArray<UVertLine*> YSubs;
	L2->RecursiveBetweenLines(L3, Subdivisions.Y, YSubs);

	// Add lines between L1,L2 and L3,L4 pairs
	TArray<UVertLine*> FrontZSubs;
	L1->RecursiveBetweenLines(L2, Subdivisions.Z, FrontZSubs);
	TArray<UVertLine*> BackZSubs;
	L3->RecursiveBetweenLines(L4, Subdivisions.Z, BackZSubs);

	Lines.Add(L1);
	Lines.Append(FrontZSubs);
	Lines.Add(L2);
	Lines.Append(YSubs);
	Lines.Add(L3);
	Lines.Append(BackZSubs);
	Lines.Add(L4);

	for (auto L : Lines)
		L->Subdivide(Subdivisions.X);

}