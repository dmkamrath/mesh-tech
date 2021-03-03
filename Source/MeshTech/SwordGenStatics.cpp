// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordGenStatics.h"

FSwordGenParams USwordGenStatics::GetSwordGenParams()
{
	FSwordGenParams P;
	P.MasterScale = 100.0;
	P.BladeParams = GetBladeParams();
	P.GuardParams = GetGuardParams();
	P.GripParams = GetGripParams();
	P.GripCapParams = GetGripCapParams();
	return P;
}

FSwordGenBladeParams USwordGenStatics::GetBladeParams()
{
	FSwordGenBladeParams P;
	P.Generics.Length = 2;
	P.Generics.Width = .2;
	P.Generics.Thickness = .025;
	P.TipGradient = 35.0;

	P.Generics.MaterialParams.bShadeFlat = true;
	P.Generics.MaterialParams.Metalic = 0.8;
	P.Generics.MaterialParams.Roughness = 0.2;
	P.Generics.MaterialParams.Color = FColor(40, 40, 40, 255);

	return P;
}

FSwordGenGuardParams USwordGenStatics::GetGuardParams()
{
	FSwordGenGuardParams P;
	P.Generics.Length = 0.1;
	P.Generics.Width = 0.5;
	P.Generics.Thickness = 0.1;

	P.Generics.MaterialParams.bShadeFlat = true;
	P.Generics.MaterialParams.Metalic = 0.5;
	P.Generics.MaterialParams.Roughness = 0.8;
	P.Generics.MaterialParams.Color = FColor(80, 80, 80, 255);

	return P;
}

FSwordGenGripParams USwordGenStatics::GetGripParams()
{
	FSwordGenGripParams P;
	P.Generics.Length = 0.6;
	P.Generics.Width = 0.10;
	P.Generics.Thickness = 0.08;

	P.Generics.MaterialParams.bShadeFlat = false;
	P.Generics.MaterialParams.Metalic = 0.0;
	P.Generics.MaterialParams.Roughness = 0.9;
	P.Generics.MaterialParams.Color = FColor(0x13, 0x0E, 0x09, 255);

	return P;
}

FSwordGenGripCapParams USwordGenStatics::GetGripCapParams()
{
	FSwordGenGripCapParams P;
	P.Generics.Length = 0.1;
	P.Generics.Width = 0.1;
	P.Generics.Thickness = 0.1;

	P.Generics.MaterialParams.bShadeFlat = true;
	P.Generics.MaterialParams.Metalic = 0.9;
	P.Generics.MaterialParams.Roughness = 0.1;
	P.Generics.MaterialParams.Color = FColor(220, 220, 220, 255);

	return P;
}