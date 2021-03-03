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

UVertLine* UVertLine::Enclose2D(FVector Center, float R, FVector Up, int32 VertCount, FVector FirstVertGuide[2])
{
	if (Up == FVector(0, 0, 0))
		return nullptr;

	Up.Normalize();

	UVertLine* L = NewObject<UVertLine>();

	FVector Ortho = GetOrthogonalVector(Up);

	// Find ortho plane and last line intercept, then multiply by R and use as first vert
	if (FirstVertGuide[0] != FVector(0, 0, 0))
	{
		FVector GuideIntersection = FMath::LinePlaneIntersection(FirstVertGuide[0], FirstVertGuide[1], Center, Up);
		Ortho = (GuideIntersection - Center).GetSafeNormal();
	}

	float Theta = 360.0 / (float)VertCount;

	for (int32 I = 0; I < VertCount; I++)
	{
		FVector V = Center + (Ortho.RotateAngleAxis(Theta * (float)I, Up) * R);
		L->Verts.Add(V);
	}

	return L;
}

TArray<UVertLine*> UVertLine::BorderLine(float SegmentThickness, int32 SegmentVertexCount)
{
	TArray<UVertLine*> EnclosureLines;
	FVector NextPlaneInterceptLine[2] = { FVector(0,0,0), FVector(0,0,0) };
	FVector D0 = (Verts[1] - Verts[0]).GetSafeNormal();
	UVertLine* L0 = Enclose2D(Verts[0], SegmentThickness, D0, SegmentVertexCount, NextPlaneInterceptLine);

	NextPlaneInterceptLine[0] = L0->GetStart();
	NextPlaneInterceptLine[1] = L0->GetStart() + D0 * 1000;

	EnclosureLines.Add(L0);
	for (int32 I = 1; I < Verts.Num() - 1; I++)
	{

		FVector Dir = (Verts[I + 1] - Verts[I - 1]).GetSafeNormal();
		UVertLine* Line = Enclose2D(Verts[I], SegmentThickness, Dir, SegmentVertexCount, NextPlaneInterceptLine);
		EnclosureLines.Add(Line);

		NextPlaneInterceptLine[0] = Line->GetStart();
		NextPlaneInterceptLine[1] = Line->GetStart() + Dir * 1000;
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
	FVector P0 = Verts[0];
	FVector P1 = ControlPoint;
	FVector P2 = Verts[Verts.Num() - 1];

	Verts.Empty();

	float Inc = 1.0 / (float)Divisions;
	
	for (int32 I = 0; I < Divisions; I++)
	{
		float T = Inc * (float)I;
		Verts.Add(GetBezierPoint(T, P0, P1, P2));
	}
	Verts.Add(P2);
}

FVector UVertLine::GetBezierPoint(float T, FVector P0, FVector P1, FVector P2)
{
	return (1 - T) * ((1 - T) * P0 + T * P1) + T * ((1 - T) * P1 + T * P2);
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