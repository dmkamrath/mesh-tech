// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MeshOps.generated.h"

/**
 * 
 */

UCLASS()
class MESHTECH_API UMeshOps : public UObject
{
	GENERATED_BODY()
public:

	static void MakeQuad(FVector Verts[4], TArray<int32> &Tris);
	
	static FVector GetMirroredPoint(FVector Point, FVector MirrorAxis);

	static void LoopCut();
};
