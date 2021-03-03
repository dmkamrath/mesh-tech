// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VertLine.generated.h"

/**
 * 
 */
UCLASS()
class MESHTECH_API UVertLine : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY()
	TArray<FVector> Verts;

	static UVertLine* Ln(FVector Start, FVector End);

	static UVertLine* Ln(TArray<FVector> LineVerts);

	static UVertLine* Enclose2D(FVector Center, float R, FVector Up, int32 VertCount, FVector FirstVertGuide[2]);

	TArray<UVertLine*> BorderLine(float SegmentThickness, int32 SegmentVertexCount);

	void Subdivide(int32 Divisions);

	void Bend(int32 Divisions, FVector ControlPoint);

	FVector GetBezierPoint(float T, FVector P0, FVector P1, FVector P2);

	static FVector GetOrthogonalVector(FVector V);

	inline FVector At(int32 Index) { return Verts[Index % Verts.Num()]; }
	inline FVector GetStart() { return Verts[0]; }
	inline FVector GetEnd() { return Verts[Verts.Num() - 1]; }
	inline int32 VertNum() { return Verts.Num(); }

};
