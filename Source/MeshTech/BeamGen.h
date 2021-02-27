// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProceduralMeshComponent.h"
#include "PointConnector.h"

#include "BeamGen.generated.h"

/**
 * 
 */
UCLASS()
class MESHTECH_API UBeamGen : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Default")
	static void GenerateBeam(FVector2D BeamDim, FVector P1, FVector P2, FRotator R, UProceduralMeshComponent* Promesh, int32 Index = 0);

	static TArray<FVector> GetBeamQuad(FVector2D BeamDim, FVector QuadCenter, FRotator R);
};
