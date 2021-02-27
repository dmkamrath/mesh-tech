// Fill out your copyright notice in the Description page of Project Settings.


#include "PromeshDriver.h"

APromeshDriver::APromeshDriver()
{
	PrimaryActorTick.bCanEverTick = true;

	Promesh = CreateDefaultSubobject<UProceduralMeshComponent>("Promesh");
	RootComponent = Promesh;

	Drag1 = CreateDefaultSubobject<USceneComponent>("Drag1");
	Drag2 = CreateDefaultSubobject<USceneComponent>("Drag2");
	Drag3 = CreateDefaultSubobject<USceneComponent>("Drag3");
	Drag4 = CreateDefaultSubobject<USceneComponent>("Drag4");
	Drag1->SetupAttachment(RootComponent);
	Drag2->SetupAttachment(RootComponent);
	Drag3->SetupAttachment(RootComponent);
	Drag4->SetupAttachment(RootComponent);

	Drag1TextComp = CreateDefaultSubobject<UTextRenderComponent>("Drag1Text");
	Drag2TextComp = CreateDefaultSubobject<UTextRenderComponent>("Drag2Text");
	Drag3TextComp = CreateDefaultSubobject<UTextRenderComponent>("Drag3Text");
	Drag4TextComp = CreateDefaultSubobject<UTextRenderComponent>("Drag4Text");

	Drag1TextComp->SetupAttachment(Drag1);
	Drag2TextComp->SetupAttachment(Drag2);
	Drag3TextComp->SetupAttachment(Drag3);
	Drag4TextComp->SetupAttachment(Drag4);

	Drag1TextComp->SetText("Drag1");
	Drag2TextComp->SetText("Drag2");
	Drag3TextComp->SetText("Drag3");
	Drag4TextComp->SetText("Drag4");
}

void APromeshDriver::GenerateMesh()
{
	//UCylinderMeshGen::GenerateCylinder(Promesh);

	FVector P1 = Drag1->GetComponentLocation() - GetActorLocation();
	FVector P2 = Drag2->GetComponentLocation() - GetActorLocation();
	FVector P3 = Drag3->GetComponentLocation() - GetActorLocation();
	FVector P4 = Drag4->GetComponentLocation() - GetActorLocation();
	//UPointConnector::Connect4P(P1, P2, P3, P4, Promesh);
	UBeamGen::GenerateBeam({ 10, 10 }, P1, P2, FRotator::ZeroRotator, Promesh);
}

void APromeshDriver::BeginPlay()
{
	Super::BeginPlay();
}

void APromeshDriver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

