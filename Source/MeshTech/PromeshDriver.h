// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "CylinderMeshGen.h"
#include "PointConnector.h"
#include "BeamGen.h"

#include "PromeshDriver.generated.h"

UCLASS()
class MESHTECH_API APromeshDriver : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default")
	UProceduralMeshComponent* Promesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default")
	USceneComponent* Drag1;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default")
	USceneComponent* Drag2;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default")
	USceneComponent* Drag3;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default")
	USceneComponent* Drag4;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default")
	UTextRenderComponent* Drag1TextComp;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default")
	UTextRenderComponent* Drag2TextComp; 
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default")
	UTextRenderComponent* Drag3TextComp; 
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default")
	UTextRenderComponent* Drag4TextComp;

	APromeshDriver();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Default")
	void GenerateMesh();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
