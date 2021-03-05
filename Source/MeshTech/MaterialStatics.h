// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PromeshBuilder.h"

#include "MaterialStatics.generated.h"

/**
 * 
 */
UCLASS()
class MESHTECH_API UMaterialStatics : public UObject
{
	GENERATED_BODY()
public:

	static FPromeshMaterialParams LightMattMetalMaterial() { return FPromeshMaterialParams(FColor(80, 80, 80, 255), true, 0.8, 0.5); }

	static FPromeshMaterialParams LightShinyMetalMaterial() { return FPromeshMaterialParams(FColor(220, 220, 220, 255), true, 0.9, 0.1); }

	static FPromeshMaterialParams DarkSheenMetalMaterial() { return FPromeshMaterialParams(FColor(40, 40, 40, 255), true, 0.8, 0.2); }

	static FPromeshMaterialParams LightShinyGoldMaterial() { return FPromeshMaterialParams(FColor(0xFF, 0xD7, 0x00, 255), true, 0.8, 0.1); }

	static FPromeshMaterialParams PaleMattGoldMaterial() { return FPromeshMaterialParams(FColor(0xD4, 0xAF, 0x37, 255), true, 0.8, 0.5); }

	static FPromeshMaterialParams DarkMattGoldMaterial() { return FPromeshMaterialParams(FColor(0xAA, 0x17, 0x00, 255), true, 0.8, 0.1); }

	static FPromeshMaterialParams DarkWoodMaterial() { return FPromeshMaterialParams(FColor(0x13, 0x0E, 0x09, 255), false, 0, 0.9); }
};
