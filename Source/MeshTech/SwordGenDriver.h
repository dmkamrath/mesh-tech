// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "SwordGen.h"
#include "SwordGenStatics.h"

#include "SwordGenDriver.generated.h"

UCLASS()
class MESHTECH_API ASwordGenDriver : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default")
	UProceduralMeshComponent* Promesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default")
	UMaterial* MaterialBase;

	UPROPERTY()
	USwordGen* SwordGen;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default")
	FSwordGenParams SwordGenParams;

	ASwordGenDriver();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Default")
	void GenerateSword();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
