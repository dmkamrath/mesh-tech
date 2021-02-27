// Fill out your copyright notice in the Description page of Project Settings.

#include "PointConnector.h"

void UPointConnector::Connect4P(FVector P1, FVector P2, FVector P3, FVector P4, UProceduralMeshComponent* Promesh, int32 Index)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;

	Vertices = { P1, P2, P3, P4 };
	Triangles = {
		2, 1, 0,
		3, 1, 2
	};
	AddDoubleSidedTris(Triangles);

	// Normalize uvs
	float Height = (P2 - P1).Size();
	float Width = (P3 - P1).Size();
	float One = FMath::Max<float>( Width, Height);
	float X = 10 * Width / One;
	float Y = 10 * Height / One;
	UVs = { {0, 0}, {0, Y}, {X, 0}, {X, Y} };

	//FVector T1Normal = FVector::CrossProduct(P3 - P1, P2 - P1).GetSafeNormal();
	//FVector T2Normal = FVector::CrossProduct(P2 - P4, P3 - P4).GetSafeNormal();
	//Normals = { T1Normal, T2Normal };

	Normals = GetAutoNormals(Vertices, Triangles);

	FVector SurfaceTangent = (P1 - P2).GetSafeNormal();
	FProcMeshTangent Tangent(SurfaceTangent, true);
	Tangents = { Tangent, Tangent, Tangent, Tangent };

	//Promesh->ClearAllMeshSections();
	Promesh->CreateMeshSection_LinearColor(Index, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
}

void UPointConnector::Connect4P(TArray<FVector> Pts, UProceduralMeshComponent* Promesh, int32 Index)
{
	Connect4P(Pts[0], Pts[1], Pts[2], Pts[3], Promesh, Index);
}

void UPointConnector::Connect8P(TArray<FVector> Pts, UProceduralMeshComponent* Promesh, int32 Index)
{
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	TArray<FColor> VertexColors;

	float Length = (Pts[0] - Pts[4]).Size();
	float Width = (Pts[0] - Pts[1]).Size();
	float Height = (Pts[0] - Pts[2]).Size();

	float UvScale = 100.0;

	TArray<int32> MainTris = {
		4, 1, 0,
		4, 5, 1,
		0, 2, 4,
		2, 6, 4,
		2, 3, 7,
		7, 6, 2,
		5, 3, 1,
		5, 7, 3
	};

	TArray<int32> CapTris = {
		0, 1, 2,
		2, 1, 3,
		6, 5, 4,
		7, 5, 6,
	};

	AddDoubleSidedTris(MainTris);
	AddDoubleSidedTris(CapTris);

	float UvY = Length / UvScale;
	float UvX = Height / UvScale;

	TArray<FVector2D> MainUvs = {
		{0, UvY}, {UvX, UvY}, {UvX, UvY}, {0, UvY},
		{0, 0}, {UvX, 0}, {UvX, 0}, {0, 0}
	};

	TArray<FVector2D> CapUvs = {
		{0, 0}, {UvX, 0}, {0, UvX}, {UvX, UvX},
		{0, 0}, {UvX, 0}, {0, UvX}, {UvX, UvX}
	};

	Promesh->CreateMeshSection(Index, Pts, MainTris, Normals, MainUvs, VertexColors, Tangents, true);
	Promesh->CreateMeshSection(Index + 1, Pts, CapTris, Normals, CapUvs, VertexColors, Tangents, true);
}

TArray<FVector> UPointConnector::GetAutoNormals(TArray<FVector> &Points, TArray<int32> &Tris, bool bDoubleSided)
{
	TArray<FVector> OutNorms;
	int32 Num = bDoubleSided ? Tris.Num() / 2 : Tris.Num();
	for (int32 I = 0; I < Num / 2; I += 3)
	{
		OutNorms.Add(GetTriNormal(Points[Tris[I]], Points[Tris[I + 1]], Points[Tris[I + 2]]));
	}
	return OutNorms;
}

FVector UPointConnector::GetTriNormal(FVector TriP1, FVector TriP2, FVector TriP3)
{
	return FVector::CrossProduct(TriP3 - TriP1, TriP2 - TriP1).GetSafeNormal();
}

void UPointConnector::AddDoubleSidedTris(TArray<int32> &Tris)
{
	int32 Num = Tris.Num();
	for (int32 I = 0; I < Num; I+=3)
	{
		int32 P1 = Tris[I + 2];
		int32 P2 = Tris[I + 1];
		int32 P3 = Tris[I];
		Tris.Append({ P1, P2, P3 });
	}
}