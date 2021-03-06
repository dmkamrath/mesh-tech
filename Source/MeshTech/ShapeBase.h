// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VertLine.h"

#include "ShapeBase.generated.h"

/*
* 
*  Made up of lines.. makes a shape
* 
* 
 */

UCLASS()
class MESHTECH_API UShapeBase : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY()
	FVector Zero = { 0, 0, 0 };

	UPROPERTY()
	FVector Dims = { 1, 1, 1 };

	UPROPERTY()
	FVector Dir = { 1, 1, 1 };

	UPROPERTY()
	FIntVector Subdivisions = { 0, 0, 0 };

	UPROPERTY()
	TArray<UVertLine*> Lines;

	virtual void InitLines();

	void Bend(int32 Divisions, FVector ControlPoint);

	void Bend(int32 Divisions, FVector ControlPoint1, FVector ControlPoint2);
};

UCLASS()
class MESHTECH_API UCubeBase : public UShapeBase
{
	GENERATED_BODY()
public:



	virtual void InitLines() override;
};