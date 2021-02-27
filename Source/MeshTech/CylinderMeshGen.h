// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProceduralMeshComponent.h"

#include "CylinderMeshGen.generated.h"

/**
 * 
 */
UCLASS()
class MESHTECH_API UCylinderMeshGen : public UObject
{
	GENERATED_BODY()
public:

	static void GenerateCylinder(UProceduralMeshComponent* Promesh);
};
