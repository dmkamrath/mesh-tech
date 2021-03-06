// Fill out your copyright notice in the Description page of Project Settings.


#include "VertLine.h"

UVertLine* UVertLine::Ln(FVector Start, FVector End)
{
	UVertLine* Vl = NewObject<UVertLine>();
	Vl->Verts.Add(Start);
	Vl->Verts.Add(End);
	return Vl;
}

UVertLine* UVertLine::Ln(TArray<FVector> LineVerts)
{
	UVertLine* Vl = NewObject<UVertLine>();
	Vl->Verts = LineVerts;
	return Vl;
}

UVertLine* UVertLine::Subline(int32 StartIndex, int32 EndOffset)
{
	UVertLine* L = NewObject<UVertLine>();
	int32 Stop = EndOffset != -1 ? Verts.Num() - EndOffset : Verts.Num();
	for (int32 I = 1; I < Stop; I++)
		L->Verts.Add(Verts[I]);
	return L;
}

UVertLine* UVertLine::TranslatedCopy(FVector Translation)
{
	UVertLine* L = NewObject<UVertLine>();
	for (auto V : Verts)
		L->Verts.Add(V + Translation);
	return L;
}

void UVertLine::Translate(FVector Translation)
{
	for (auto V : Verts)
		V += Translation;
}

UVertLine* UVertLine::GetMirroredLine(FVector MirrorAxis)
{
	UVertLine* L = NewObject<UVertLine>();
	for (auto V : Verts)
		L->Verts.Add(V * MirrorAxis);
	return L;
}

UVertLine* UVertLine::GetBetweenLine(UVertLine* OtherLine)
{
	UVertLine* L = NewObject<UVertLine>();
	int32 N = FMath::Min(Verts.Num(), OtherLine->VertNum());
	for (int32 I = 0; I < N; I++)
	{
		// Changing the divisor here does cool stuff for the recursive case
		L->Verts.Add((Verts[I] + OtherLine->Verts[I]) / 2.01);
	}
	return L;
}

void UVertLine::RecursiveBetweenLines(UVertLine* Other, int32 N, TArray<UVertLine*>& OutLines)
{
	if (N == 0)
		return;

	UVertLine* L = GetBetweenLine(Other);
	RecursiveBetweenLines(L, N - 1, OutLines);
	L->RecursiveBetweenLines(Other, N - 1, OutLines);
	OutLines.Add(L);
}

UVertLine* UVertLine::Enclose2D(FVector Center, float R, FVector Up, int32 VertCount, FVector FirstVertGuide[2])
{
	return UVertLine::Ln(UVecOps::NPointEnclosure2D(Center, R, Up, VertCount, FirstVertGuide));
}

TArray<UVertLine*> UVertLine::BorderLine(float SegmentThickness, int32 SegmentVertexCount, float ThicknessDecay)
{
	TArray<UVertLine*> EnclosureLines;

	FVector NextPlaneInterceptLine[2] = { FVector(0,0,0), FVector(0,0,0) };
	FVector D0 = (Verts[1] - Verts[0]).GetSafeNormal();
	UVertLine* L0 = Enclose2D(Verts[0], SegmentThickness, D0, SegmentVertexCount, NextPlaneInterceptLine);
	SegmentThickness *= ThicknessDecay;

	NextPlaneInterceptLine[0] = L0->GetStart();
	NextPlaneInterceptLine[1] = L0->GetStart() + D0 * 1000;

	EnclosureLines.Add(L0);
	for (int32 I = 1; I < Verts.Num() - 1; I++)
	{
		FVector Dir = (Verts[I + 1] - Verts[I - 1]).GetSafeNormal();
		UVertLine* Line = Enclose2D(Verts[I], SegmentThickness, Dir, SegmentVertexCount, NextPlaneInterceptLine);
		EnclosureLines.Add(Line);

		NextPlaneInterceptLine[0] = Line->GetStart();
		// Dont use tangent here, use direction from this center to next center ?
		// NextPlaneInterceptLine[1] = Line->GetStart() + Dir * 1000;
		NextPlaneInterceptLine[1] = Line->GetStart() + (Verts[I + 1] - Verts[I]).GetSafeNormal() * 1000;
		SegmentThickness *= ThicknessDecay;
	}
	FVector DN = (Verts[Verts.Num() - 1] - Verts[Verts.Num() - 2]).GetSafeNormal();
	UVertLine* LN = Enclose2D(Verts[Verts.Num() - 1], SegmentThickness, DN, SegmentVertexCount, NextPlaneInterceptLine);
	EnclosureLines.Add(LN);
	
	return EnclosureLines;
}

void UVertLine::Subdivide(int32 Divisions)
{
	TArray<FVector> NewVerts;
	for (int32 I = 0; I < Verts.Num() - 1; I++)
	{
		NewVerts.Add(Verts[I]);
		float Len;
		FVector Dir;
		(Verts[I + 1] - Verts[I]).ToDirectionAndLength(Dir, Len);
		float Inc = Len / (float)Divisions;
		for (int32 D = 0; D < Divisions; D++)
		{
			float Dist = Inc * (float)D + 1;
			FVector V = Verts[I] + Dir * Dist;
			NewVerts.Add(V);
		}
		NewVerts.Add(Verts[I + 1]);
	}
	Verts = NewVerts;
}

void UVertLine::Bend(int32 Divisions, FVector ControlPoint)
{
	FVector P0 = GetStart();
	FVector P2 = GetEnd();
	Verts.Empty();
	AddBezierPoints(Divisions, P0, ControlPoint, P2);
}

void UVertLine::Bend(int32 Divisions, FVector ControlPoint1, FVector ControlPoint2)
{
	FVector P0 = GetStart();
	FVector P3 = GetEnd();
	Verts.Empty();
	AddBezierPoints(Divisions, P0, ControlPoint1, ControlPoint2, P3);
}

void UVertLine::ClampVertsForMirrorAxis(FIntVector MirrorAxis)
{
	// For the set axis (which is negative) find the majority sign of verts, then set any that cross it to zero

	// Maybe set all verts to zero if one fails the check ?

	// Need extra reference verts / lines

	if (MirrorAxis[0] == -1)
	{
		bool bViolatedX = false;
		float Sum = 0;
		for (auto V : Verts)
			Sum += FMath::Sign(V[0]);
		for (int32 I = 0; I < Verts.Num(); I++)
			if (FMath::Sign(Verts[I][0]) == -FMath::Sign(Sum))
				bViolatedX = true;
		if (bViolatedX)
			for (int32 I = 0; I < Verts.Num(); I++)
				Verts[I][0] = 0;
	}
	if (MirrorAxis[1] == -1)
	{
		bool bViolatedY = false;
		float Sum = 0;
		for (auto V : Verts)
			Sum += FMath::Sign(V[1]);
		for (int32 I = 0; I < Verts.Num(); I++)
			if (FMath::Sign(Verts[I][1]) ==  -FMath::Sign(Sum))
				bViolatedY = true;
		if (bViolatedY)
			for (int32 I = 0; I < Verts.Num(); I++)
				Verts[I][1] = 0;
	}
	if (MirrorAxis[2] == -1)
	{
		bool bViolatedZ = false;
		float Sum = 0;
		for (auto V : Verts)
			Sum += FMath::Sign(V[2]);
		for (int32 I = 0; I < Verts.Num(); I++)
			if (FMath::Sign(Verts[I][2]) == -FMath::Sign(Sum))
				bViolatedZ = true;
		if (bViolatedZ)
			for (int32 I = 0; I < Verts.Num(); I++)
				Verts[I][2] = 0;
	}
}

void UVertLine::AddBezierPoints(int32 N, FVector P0, FVector P1, FVector P2)
{
	float Inc = 1.0 / (float)N;
	for (int32 I = 0; I < N; I++)
	{
		float T = Inc * (float)I;
		Verts.Add(GetBezierPoint(T, P0, P1, P2));
	}
	Verts.Add(P2);
}

void UVertLine::AddBezierPoints(int32 N, FVector P0, FVector P1, FVector P2, FVector P3)
{
	float Inc = 1.0 / (float)N;
	for (int32 I = 0; I < N; I++)
	{
		float T = Inc * (float)I;
		Verts.Add(GetBezierPoint(T, P0, P1, P2, P3));
	}
	//Verts.Add(P3);
}

FVector UVertLine::GetBezierPoint(float T, FVector P0, FVector P1, FVector P2)
{
	return (1 - T) * ((1 - T) * P0 + T * P1) + T * ((1 - T) * P1 + T * P2);
}

FVector UVertLine::GetBezierPoint(float T, FVector P0, FVector P1, FVector P2, FVector P3)
{
	float Z = 1 - T;
	float Zsq = Z * Z;
	return Z * Zsq * P0 + 3.0 * Zsq * T * P1 + 3.0 * Z * T * T * P2 + T * T * T * P3;
}

FVector UVertLine::GetCenter()
{
	FVector Acc(0,0,0);
	for (auto V : Verts)
		Acc += V;
	return Acc / Verts.Num();
}

UVertLine* UVertLine::GetCenterPointAsLine()
{
	UVertLine* L = NewObject<UVertLine>();
	L->Verts.Add(GetCenter());
	return L;
}

FVector UVertLine::GetOrthogonalVector(FVector V)
{
	FVector Ortho(0,0,0);
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

void UVertLine::VectorMaxComponentDir(FVector& V)
{
	for (int32 I = 0; I < 3; I++)
		if (FMath::Abs(V[I]) != V.GetAbsMax())
			V[I] = 0;
	V.Normalize();
}