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

	UVertLine* Subline(int32 StartIndex, int32 EndOffset = -1);

	UVertLine* TranslatedCopy(FVector Translation);

	void Translate(FVector Translation);

	UVertLine* GetMirroredLine(FVector MirrorAxis);

	UVertLine* GetBetweenLine(UVertLine* OtherLine);

	void RecursiveBetweenLines(UVertLine* Other, int32 N, TArray<UVertLine*>& OutLines);

	static UVertLine* Enclose2D(FVector Center, float R, FVector Up, int32 VertCount, FVector FirstVertGuide[2]);

	TArray<UVertLine*> BorderLine(float SegmentThickness, int32 SegmentVertexCount, float ThicknessDecay = 1.0);

	void Subdivide(int32 Divisions);

	void Bend(int32 Divisions, FVector ControlPoint);

	void Bend(int32 Divisions, FVector ControlPoint1, FVector ControlPoint2);

	void BendKeepStraightEnds(int32 Divisions, FVector ControlPoint);

	void AddStartEndTangentPoints();

	void ClampVertsForMirrorAxis(FIntVector MirrorAxis);

	void AddBezierPoints(int32 N, FVector P0, FVector P1, FVector P2);

	void AddBezierPoints(int32 N, FVector P0, FVector P1, FVector P2, FVector P3);

	FVector GetBezierPoint(float T, FVector P0, FVector P1, FVector P2);

	FVector GetBezierPoint(float T, FVector P0, FVector P1, FVector P2, FVector P3);

	FVector GetCenter();

	UVertLine* GetCenterPointAsLine();

	static FVector GetOrthogonalVector(FVector V);

	static void SnapVector(FVector& V, float SnapThresh);

	static void VectorMaxComponentDir(FVector& V);


	inline FVector At(int32 Index) { return Verts[Index % Verts.Num()]; }
	inline FVector GetStart() { return Verts[0]; }
	inline FVector GetEnd() { return Verts[Verts.Num() - 1]; }
	inline int32 VertNum() { return Verts.Num(); }

};
