// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SwordGen.h"

#include "SwordGenStatics.generated.h"

/**
 * 
 */
UCLASS()
class MESHTECH_API USwordGenStatics : public UObject
{
	GENERATED_BODY()
public:

	static FSwordGenParams GetSwordGenParams();

	static FSwordGenBladeParams GetBladeParams();

	static FSwordGenGuardParams GetGuardParams();

	static FSwordGenGripParams GetGripParams();

	static FSwordGenGripCapParams GetGripCapParams();
};
