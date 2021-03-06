// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordGenDriver.h"

ASwordGenDriver::ASwordGenDriver()
{
	PrimaryActorTick.bCanEverTick = true;
	Promesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Promesh"));
	RootComponent = Promesh;
	SwordGen = CreateDefaultSubobject<USwordGen>(TEXT("SwordGen"));
	SwordGen->Promesh = Promesh;

	SwordGenParams = USwordGenStatics::GetSwordGenParams();
	SwordGenParams.MasterScale = 1000;

}

void ASwordGenDriver::GenerateSword()
{
	SwordGen->SetParams(SwordGenParams);
	SwordGen->MaterialBase = MaterialBase;
	SwordGen->Generate();
}

void ASwordGenDriver::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASwordGenDriver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

