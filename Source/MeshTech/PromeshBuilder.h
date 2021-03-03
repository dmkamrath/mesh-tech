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

	void MakeMirroredQuad(FVector QuadVerts[4], uint8 MirrorAxis);

	void MakeTri(FVector TriVerts[3]);

	void MakeMirroredTri(FVector TriVerts[3], uint8 MirrorAxis);

	void StitchLines(UVertLine* L1, UVertLine* L2, bool bConnectEnds = false, uint8 MirrorAxis = 0);

	void StitchLineSequence(TArray<UVertLine*> Lines, bool bConnectEnds = false, uint8 MirrorAxis = 0);

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
