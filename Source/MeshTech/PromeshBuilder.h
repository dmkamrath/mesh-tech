// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProceduralMeshComponent.h"
#include "VertLine.h"

#include "PromeshBuilder.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPromeshMaterialParams
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	FColor Color = FColor::White;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	bool bShadeFlat = true;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	float Metalic = 0.0;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	float Specular = 0.0;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	float Roughness = 1.0;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	float Emissive = 0.0;

	FPromeshMaterialParams() {}
	FPromeshMaterialParams(FColor C, bool bFlat, float M, float R, float E = 0.0, float S = 0.0)
	{
		Color = C;
		bShadeFlat = bFlat;
		Metalic = M;
		Specular = S;
		Roughness = R;
		Emissive = E;
	}

	void Apply(UMaterialInstanceDynamic* Dmat)
	{
		Dmat->SetScalarParameterValue("metalic", Metalic);
		Dmat->SetScalarParameterValue("specular", Specular);
		Dmat->SetScalarParameterValue("roughness", Roughness);
		Dmat->SetScalarParameterValue("emissive", Emissive);
	}
	
};

enum EMirrorAxis
{
	X = 0b1,
	Y = 0b10,
	Z = 0b100
};

struct FMirror
{
	static FVector Vec(uint8 MirrorAxis)
	{
		if (MirrorAxis & EMirrorAxis::X)
			return { 1, -1, 1 };
		if (MirrorAxis & EMirrorAxis::Y)
			return { -1, 1, 1 };
		if (MirrorAxis & EMirrorAxis::Z)
			return { 1, 1, -1 };
		return { 1,1,1 };
	}
};

USTRUCT()
struct FVertexNormal
{
	GENERATED_USTRUCT_BODY()
public:

	TArray<FVector> FaceNormals;

	void AddFaceNormal(FVector FaceNormal)
	{
		FaceNormals.Add(FaceNormal);
	}

	FVector Get()
	{
		FVector Normal(0,0,0);
		for (auto N : FaceNormals)
			Normal += N;
		return Normal / FaceNormals.Num();
	}
};

USTRUCT()
struct FTri
{
	GENERATED_USTRUCT_BODY()
public:

	FVector P0;
	FVector P1;
	FVector P2;

	FTri() {};
	FTri(FVector TriP0, FVector TriP1, FVector TriP2)
	{
		P0 = TriP0;
		P1 = TriP1;
		P2 = TriP2;
	}

	FTri Invert()
	{
		return FTri(P2, P1, P0);
	}

	FTri Mirror(FVector Axis, bool bClamp)
	{
		return FTri(P0 * Axis, P1 * Axis, P2 * Axis);
	}

	FVector GetNormal()
	{
		FVector A = P2 - P0;
		FVector B = P1 - P0;
		return FVector::CrossProduct(A, B).GetSafeNormal();
	}

	FVector& operator[] (int32 Index)
	{
		switch (Index)
		{
		case 0:
			return P0;
		case 1:
			return P1;
		}
		return P2;
	}
};

UCLASS()
class MESHTECH_API UPromeshBuilder : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY()
	FPromeshMaterialParams MaterialParams;

	UPROPERTY()
	UMaterial* MaterialBase;

	UPROPERTY()
	float SameVertThresh;

	UPROPERTY()
	TArray<FVector> Verts;

	UPROPERTY()
	TArray<FVertexNormal> VertNormals;

	UPROPERTY()
	TArray<int32> Tris;

	UPROPERTY()
	TArray<FColor> VertexColors;

	UPROPERTY()
	TArray<FVector2D> Uvs;

	bool bDoubleSided;

	UPromeshBuilder();

	void BuildMesh(UProceduralMeshComponent* Promesh, int32 SectionIndex = 0);

	void ApplyMaterialParams(UProceduralMeshComponent* Promesh, int32 SectionIndex = 0);

	void Reset();

	int32 AddVert(FVector Vert);

	bool HasVert(FVector Vert);

	int32 ExpressAddVert(FVector Vert);

	int32 GetVertIndex(FVector Vert);

	void MakeQuad(FVector QuadVerts[4]);

	void MakeQuad(FVector QuadVerts[4], uint8 MirrorAxis);

	void MakeTri(FTri Tri);

	void MakeTri(FTri Tri, uint8 MirrorAxis);

	void StitchLines(UVertLine* L1, UVertLine* L2, bool bConnectEnds = false, uint8 MirrorAxis = 0);

	void StitchLines(TArray<UVertLine*> Lines, bool bConnectEnds = false, uint8 MirrorAxis = 0);

	void ClampLinesForMirror(TArray<UVertLine*> Lines, uint8 MirrorAxis);

	void AddBorderLines(UVertLine* CenterLine, float SegmentThickness, int32 SegmentVertCount, uint8 MirrorAxis = 0, float ThicknessDecay = 1.0);

	void AddDecoBorderLines(UVertLine* CenterLine, float SegmentThickness, int32 SegmentVertCount, FPromeshMaterialParams DecoMaterialParams, uint8 MirrorAxis = 0, float ThicknessDecay = 1.0);

	void SetMaterialParams(FPromeshMaterialParams NewParams);

	void MakeDoubleSided();

	TArray<FVector> GetVerts();

	TArray<int32> GetTris();

	TArray<FVector> GetNormals();

	FVector GetTriNormal(FVector TriVerts[3]);

	TArray<FVector2D> GetUvs();

	TArray<FColor> GetVertexColors();

	TArray<FProcMeshTangent> GetTangents();

	UMaterialInterface* GetMaterialBase();

};
