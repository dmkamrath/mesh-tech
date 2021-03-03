// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcMeshTri.h"

FVector UProcMeshTri::GetNormal()
{
	return FVector::CrossProduct(Verts[2] - Verts[0], Verts[1] - Verts[0]).GetSafeNormal();
}
