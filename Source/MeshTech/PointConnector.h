// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProceduralMeshComponent.h"

#include "PromeshBuilder.h"

#include "PointConnector.generated.h"


UCLASS()
class MESHTECH_API UPointConnector : public UObject
{
	GENERATED_BODY()
public:

	static void Connect4P(FVector P1, FVector P2, FVector P3, FVector P4, UProceduralMeshComponent* Promesh, int32 Index = 0);

	UFUNCTION(BlueprintCallable, Category = "Point Connector")
	static void Connect4P(TArray<FVector> Pts, UProceduralMeshComponent* Promesh, int32 Index = 0);

	static void Connect8P(TArray<FVector> Pts, UProceduralMeshComponent* Promesh, int32 Index = 0);

	static TArray<FVector> GetAutoNormals(TArray<FVector> &Points, TArray<int32> &Tris, bool bDoubleSided = true);

	static FVector GetTriNormal(FVector TriP1, FVector TriP2, FVector TriP3);

	static void AddDoubleSidedTris(TArray<int32> &Tris);
};
