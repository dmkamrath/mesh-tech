// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshOps.h"

void UMeshOps::MakeQuad(FVector Verts[4], TArray<int32>& Tris)
{
	//Tris.Add(Verts[0]);
	//Tris.Add(Verts[1]);
	//Tris.Add(Verts[2]);
	//Tris.Add(Verts[0]);
	//Tris.Add(Verts[2]);
	//Tris.Add(Verts[3]);
}

FVector UMeshOps::GetMirroredPoint(FVector Point, FVector MirrorAxis)
{
	FPlane P;
	return Point.MirrorByVector(MirrorAxis);
}

void UMeshOps::LoopCut()
{
	FVector P1;
	FVector P2;
	FVector CutOrigin(0, 0, 0);
	FVector CutNormal;
	FPlane(CutOrigin, CutNormal);
	FVector CutPoint = FMath::LinePlaneIntersection(P1, P2, CutOrigin, CutNormal);

	// for each tri, check verts to see if two are on seperate sides of they mirror plane.

	// if so, find intersect and figure out how to split up the tris
}