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
	P.Generics.MaterialParams = UMaterialStatics::DarkSheenMetalMaterial();

	return P;
}

FSwordGenGuardParams USwordGenStatics::GetGuardParams()
{
	FSwordGenGuardParams P;
	P.Generics.Length = 0.1;
	P.Generics.Width = 0.5;
	P.Generics.Thickness = 0.1;
	P.Generics.MaterialParams = UMaterialStatics::LightMattMetalMaterial();

	return P;
}

FSwordGenGripParams USwordGenStatics::GetGripParams()
{
	FSwordGenGripParams P;
	P.Generics.Length = 0.6;
	P.Generics.Width = 0.10;
	P.Generics.Thickness = 0.08;
	P.Generics.MaterialParams = UMaterialStatics::DarkWoodMaterial();

	return P;
}

FSwordGenGripCapParams USwordGenStatics::GetGripCapParams()
{
	FSwordGenGripCapParams P;
	P.Generics.Length = 0.1;
	P.Generics.Width = 0.1;
	P.Generics.Thickness = 0.1;
	P.Generics.MaterialParams = UMaterialStatics::DarkMattGoldMaterial();

	return P;
}