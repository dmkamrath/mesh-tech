// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProceduralMeshComponent.h"

#include "PointConnector.h"
#include "BeamGen.h"
#include "PromeshBuilder.h"
#include "ShapeBase.h"
#include "MaterialStatics.h"

#include "SwordGen.generated.h"

/**
 * 
 */

UENUM()
enum class ESwordGenPart : uint8
{
	Blade,
	Guard,
	GuardCaps,
	Grip,
	GripCap,
	Scabbard
};

static const int32 ESwordGenPart_COUNT = static_cast<int32>(ESwordGenPart::Scabbard) + 1;


USTRUCT(Blueprintable)
struct FSwordGenGenericParams
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	FPromeshMaterialParams MaterialParams;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	float Length = 2.0;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	float Width = 0.2;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	float Thickness = 0.05;
};

USTRUCT(Blueprintable)
struct FSwordGenBladeParams
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	FSwordGenGenericParams Generics;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	float EdgeGradient = 45.0;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	float TipGradient = 45.0;

};

USTRUCT(Blueprintable)
struct FSwordGenGuardParams
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	FSwordGenGenericParams Generics;

};

USTRUCT(Blueprintable)
struct FSwordGenGripParams
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	FSwordGenGenericParams Generics;

};

USTRUCT(Blueprintable)
struct FSwordGenGripCapParams
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	FSwordGenGenericParams Generics;

};


USTRUCT(Blueprintable)
struct FSwordGenParams
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	float MasterScale = 100.0;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	FSwordGenBladeParams BladeParams;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	FSwordGenGuardParams GuardParams;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	FSwordGenGripParams GripParams;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Default")
	FSwordGenGripCapParams GripCapParams;

};

UCLASS()
class MESHTECH_API USwordGen : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = "Default")
	UProceduralMeshComponent* Promesh;

	UPROPERTY(BlueprintReadOnly, Category = "Default")
	UMaterial* MaterialBase;

	UPROPERTY(BlueprintReadOnly, Category = "Default")
	float MasterScale = 1.0;

	UPROPERTY(BlueprintReadOnly, Category = "Default")
	FSwordGenParams Params;

	UPROPERTY()
	UPromeshBuilder* PromeshBuilder;

	USwordGen();

	void SetParams(FSwordGenParams NewParams);

	void Generate();

	void GenerateBlade();

	void GenerateGuard();

	void GenerateGrip();

	void GenerateGripCap();
	
	void GenerateDeco();

	/*
			Blade Parameters
	*/

	float GetBladeLength();

	float GetBladeWidth();

	float GetBladeThickness();

	float GetBladeEdgeGradient();

	float GetBladeTipGradient();

	FPromeshMaterialParams GetBladeMaterialParams();

	/*
			Guard Parameters
	*/

	float GetGuardLength();

	float GetGuardWidth();

	float GetGuardThickness();

	FPromeshMaterialParams GetGuardMaterialParams();

	/*
			Grip Parameters
	*/

	float GetGripLength();

	float GetGripWidth();

	float GetGripThickness();

	FColor GetGripColor();

	FPromeshMaterialParams GetGripMaterialParams();

	/*
			Grip Cap Parameters
	*/

	float GetGripCapLength();

	float GetGripCapWidth();

	float GetGripCapThickness();

	FPromeshMaterialParams GetGripCapMaterialParams();

};
