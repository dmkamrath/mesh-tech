// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProceduralMeshComponent.h"

#include "ProcMeshTri.generated.h"

/**
 * 
 */
UCLASS()
class MESHTECH_API UProcMeshTri : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY()
	FVector Verts[3];

	FProcMeshTangent Tangent;

	UFUNCTION()
	FVector GetNormal();
	
};
