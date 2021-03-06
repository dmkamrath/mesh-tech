// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordGen.h"

USwordGen::USwordGen()
{
	PromeshBuilder = CreateDefaultSubobject<UPromeshBuilder>(TEXT("Promesh Builder"));
}

void USwordGen::SetParams(FSwordGenParams NewParams)
{
	Params = NewParams;
}

void USwordGen::Generate()
{
	PromeshBuilder->MaterialBase = MaterialBase;

	GenerateBlade();
	GenerateGuard();
	GenerateGrip();
	GenerateGripCap();
	GenerateDeco();

}

void USwordGen::GenerateBlade()
{
	PromeshBuilder->SetMaterialParams(GetBladeMaterialParams());


	float HalfThick = GetBladeThickness() / 2.0;

	FVector Tip = { GetBladeLength(), 0.0, 0.0 };
	FVector BaseCenter = { 0.0, 0.0, 0.0 };
	FVector BaseEdge = { 0.0, GetBladeWidth() / 2.0f, 0.0 };
	FVector BaseTop = { 0.0, 0.0, HalfThick };

	float TaperHeight1 = GetBladeLength() - (GetBladeWidth() / FMath::Tan(FMath::DegreesToRadians(GetBladeTipGradient())));

	FVector TaperCenter1 = { TaperHeight1, 0.0, HalfThick };
	FVector TaperEdge1 = { TaperHeight1, GetBladeWidth() / 2.0f, 0.0};


	// Taper subdivisions

	float TaperLen;
	FVector TaperDir;
	(TaperEdge1 - Tip).ToDirectionAndLength(TaperDir, TaperLen);

	FVector TaperMidEdge = Tip + TaperDir * TaperLen * 0.5;
	TaperMidEdge.Y += GetBladeWidth() / 10;
	FVector TaperMidCenter = { TaperMidEdge.X, 0.0, HalfThick };

	// Each section of the blade will be a "line" of points to be stitched to a line below it

	int32 NumTaperDivisions = 4;

	TArray<FVector> TaperEdges;
	TArray<FVector> TaperCenters;
	for (int32 I = 0; I < NumTaperDivisions; I++)
	{
		FVector TaperDivEdge = Tip + TaperDir * TaperLen * ((float)I / NumTaperDivisions);
		FVector TaperDivCenter = { TaperDivEdge.X, 0.0, HalfThick };
		TaperEdges.Add(TaperDivEdge);
		TaperCenters.Add(TaperDivCenter);
	}

	FVector TipTri[3] = { Tip, TaperMidCenter, TaperMidEdge };

	FVector TaperQuad[4] = { TaperMidCenter, TaperCenter1, TaperEdge1, TaperMidEdge };

	FVector EdgeQuad[4] = { TaperCenter1, BaseTop, BaseEdge, TaperEdge1 };

	




	uint8 MirrorAxis = EMirrorAxis::X | EMirrorAxis::Z;
	PromeshBuilder->MakeMirroredTri(TipTri, MirrorAxis);
	PromeshBuilder->MakeMirroredQuad(TaperQuad, MirrorAxis);
	PromeshBuilder->MakeMirroredQuad(EdgeQuad, MirrorAxis);

	// Blade deco experiments

	UVertLine* BladeRidge = UVertLine::Ln(BaseTop, TaperMidCenter);
	//PromeshBuilder->AddBorderLines(BladeRidge, 1.5, 5, EMirrorAxis::Z, 0.05);


	PromeshBuilder->BuildMesh(Promesh, 0);

	PromeshBuilder->AddDecoBorderLines(BladeRidge, 0.0045 * GetMasterScale(), 5, UMaterialStatics::DarkMattGoldMaterial(), EMirrorAxis::Z, 0.05);
	PromeshBuilder->BuildMesh(Promesh, 9);

}

void USwordGen::GenerateGuard()
{
	PromeshBuilder->SetMaterialParams(GetGuardMaterialParams());

	float Z = 0.0;

	float F = Z + GetGuardLength();
	float M = Z + GetGuardLength() / 2.0;
	float B = Z;

	float C = 0.0;
	float R = GetGuardWidth() / 2.0;

	float U = GetGuardThickness() / 2.0;
	float D = 0.0;

	FVector FCD = { F, C, D };
	FVector FRD = { F, R, D };
	FVector FCU = { F, C, U };
	FVector FRU = { F, R*.9f, U*.8f };
	FVector FRUM = { F, R*.95f, U*.7f };

	FVector MCU = { M, C, U };
	FVector MRD = { M, R, D };
	FVector MRU = { M, R*.9f, U*.8f };
	FVector MRUM = { M, R * .95f, U * .7f };

	FVector BCD = { B, C, D };
	FVector BRD = { B, R, D };
	FVector BCU = { B, C, U };
	FVector BRU = { B, R*.9f, U*.8f };
	FVector BRUM = { B, R * .95f, U * .7f };

	FVector Q1[4] = { FCD, FCU, FRU, FRD };
	FVector Q2[4] = { FCU, MCU, MRU, FRU };
	FVector Q3[4] = { MCU, BCU, BRU, MRU };
	FVector Q4[4] = { BCU, BCD, BRD, BRU };
	FVector Q5[4] = { FRU, BRU, BRD, FRD };

	uint8 Mirror = EMirrorAxis::X | EMirrorAxis::Z;
	/*PromeshBuilder->MakeMirroredQuad(Q1, Mirror);
	PromeshBuilder->MakeMirroredQuad(Q2, Mirror);
	PromeshBuilder->MakeMirroredQuad(Q3, Mirror);
	PromeshBuilder->MakeMirroredQuad(Q4, Mirror);
	PromeshBuilder->MakeMirroredQuad(Q5, Mirror);*/

	UVertLine* L1 = UVertLine::Ln(FCD, FRD);
	UVertLine* L2 = UVertLine::Ln({ FCU, FRU, FRUM, FRD });
	UVertLine* L3 = UVertLine::Ln({ BCU, BRU, BRUM, BRD });
	UVertLine* L4 = UVertLine::Ln(BCD, BRD);


	//L2->Bend(10, FRUM);
	//L3->Bend(10, BRUM);

	FRD.X -= 0.060 * GetMasterScale();
	FRD.Z += 0.04 * GetMasterScale();
	L1->Bend(10, FRD);
	L2->Bend(10, FRD);
	L3->Bend(10, FRD);
	L4->Bend(10, FRD);

	

	TArray<UVertLine*> Lines = { L1, L2, L3, L4 };
	PromeshBuilder->StitchLineSequence(Lines, false, Mirror);



	PromeshBuilder->BuildMesh(Promesh, 1);
	float BorderThickness = 0.025 * GetMasterScale();
	PromeshBuilder->AddDecoBorderLines(L2, BorderThickness, 7, UMaterialStatics::DarkMattGoldMaterial(), Mirror, 0.75);
	PromeshBuilder->AddDecoBorderLines(L3, BorderThickness*.5, 7, UMaterialStatics::DarkMattGoldMaterial(), Mirror, 0.85);
	PromeshBuilder->BuildMesh(Promesh, 7);
}

void USwordGen::GenerateGrip()
{
	PromeshBuilder->SetMaterialParams(GetGripMaterialParams());

	float F = 0.0;
	float M = -GetGripLength() / 2.0;
	float B = -GetGripLength();

	float C = 0.0;
	float R = GetGripWidth() / 2.0;

	float U = GetGripThickness() / 2.0;
	float D = 0.0;

	FVector FCD = { F, C, D };
	FVector FRD = { F, R, D };
	FVector FCU = { F, C, U };
	FVector FRU = { F, R * .7f, U * .7f };

	FVector MCU = { M, C, U };
	FVector MRD = { M, R, D };
	FVector MRU = { M, R * .7f, U * .7f };

	FVector BCD = { B, C, D };
	FVector BRD = { B, R, D };
	FVector BCU = { B, C, U };
	FVector BRU = { B, R * .7f, U * .7f };

	FVector Q2[4] = { FCU, MCU, MRU, FRU };
	FVector Q3[4] = { MCU, BCU, BRU, MRU };
	FVector Q5[4] = { FRU, BRU, BRD, FRD };

	uint8 Mirror = EMirrorAxis::X | EMirrorAxis::Z;
	PromeshBuilder->MakeMirroredQuad(Q2, Mirror);
	PromeshBuilder->MakeMirroredQuad(Q3, Mirror);
	PromeshBuilder->MakeMirroredQuad(Q5, Mirror);
	PromeshBuilder->BuildMesh(Promesh, 2);
}

void USwordGen::GenerateGripCap()
{
	PromeshBuilder->SetMaterialParams(GetGripCapMaterialParams());

	float Z = -GetGripLength();

	float F = Z;
	float M = Z - GetGripCapLength() / 2.0;
	float B = Z - GetGripCapLength();

	float C = 0.0;
	float R = GetGripCapWidth() / 2.0;

	float U = GetGripCapThickness() / 2.0;
	float D = 0.0;

	FVector FCD = { F, C, D };
	FVector FRD = { F, R, D };
	FVector FCU = { F, C, U };
	FVector FRU = { F, R*.8f, U*.8f };

	FVector MCU = { M, C, U };
	FVector MRD = { M, R, D };
	FVector MRU = { M, R*.8f, U*.8f };

	FVector BCD = { B, C, D };
	FVector BRD = { B, R, D };
	FVector BCU = { B, C, U };
	FVector BRU = { B, R*.8f, U*.8f };

	/*FVector Q1[4] = { FCD, FCU, FRU, FRD };
	FVector Q2[4] = { FCU, MCU, MRU, FRU };
	FVector Q3[4] = { MCU, BCU, BRU, MRU };
	FVector Q4[4] = { BCU, BCD, BRD, BRU };
	FVector Q5[4] = { FRU, BRU, BRD, FRD };*/

	uint8 Mirror = EMirrorAxis::X | EMirrorAxis::Z;
	/*PromeshBuilder->MakeMirroredQuad(Q1, Mirror);
	PromeshBuilder->MakeMirroredQuad(Q2, Mirror);
	PromeshBuilder->MakeMirroredQuad(Q3, Mirror);
	PromeshBuilder->MakeMirroredQuad(Q4, Mirror);
	PromeshBuilder->MakeMirroredQuad(Q5, Mirror);*/


	// Try with lines now!

	UVertLine* L1 = UVertLine::Ln({ FCD, FRD });
	UVertLine* L2 = UVertLine::Ln({ FCU, FRU, FRD });
	UVertLine* L3 = UVertLine::Ln({ BCU, BRU, BRD });
	UVertLine* L4 = UVertLine::Ln({ BCD, BRD });

	FVector ControlPoint = MRU;
	ControlPoint.Y += 2;
	ControlPoint.Z += 1;
	//ControlPoint.X -= 10;
	//L1->Subdivide(4);
	//L2->Bend(4, ControlPoint);
	//L3->BendKeepStraightEnds(4, ControlPoint);
	//L4->Bend(4, ControlPoint);
	//L5->Subdivide(4);
	
	float BorderThickness = 0.01 * GetMasterScale();
	PromeshBuilder->AddBorderLines(L2, BorderThickness, 5, Mirror);
	PromeshBuilder->AddBorderLines(L3, BorderThickness, 5, Mirror);

	TArray<UVertLine*> LineSeq = { L1, L2, L3, L4 };
	PromeshBuilder->StitchLineSequence(LineSeq, false, Mirror);


	PromeshBuilder->BuildMesh(Promesh, 3);
}

void USwordGen::GenerateDeco()
{
	UCubeBase* Cube = NewObject<UCubeBase>();
	Cube->Dims = { 20, 20, 10 };
	Cube->Dir = { 1, 1, 1 };
	Cube->InitLines();
	FVector Cp = { 20, 10, 12 };
	FVector Cp2 = { 25, 45, 11 };
	Cube->Bend(15, Cp, Cp2);
	uint8 Mirror = EMirrorAxis::X | EMirrorAxis::Z;
	PromeshBuilder->StitchLineSequence(Cube->Lines, false, Mirror);
	PromeshBuilder->BuildMesh(Promesh, 4);
}

float USwordGen::GetMasterScale()
{
	return Params.MasterScale;
}

/*
			Blade Parameters
*/

float USwordGen::GetBladeLength()
{
	return Params.BladeParams.Generics.Length * Params.MasterScale;
}

float USwordGen::GetBladeWidth()
{
	return Params.BladeParams.Generics.Width * Params.MasterScale;
}

float USwordGen::GetBladeThickness()
{
	return Params.BladeParams.Generics.Thickness * Params.MasterScale;
}

float USwordGen::GetBladeEdgeGradient()
{
	return Params.BladeParams.EdgeGradient;
}

float USwordGen::GetBladeTipGradient()
{
	return Params.BladeParams.TipGradient;
}

FPromeshMaterialParams USwordGen::GetBladeMaterialParams()
{
	return Params.BladeParams.Generics.MaterialParams;
}

/*
			Guard Parameters
*/

float USwordGen::GetGuardLength()
{
	return Params.GuardParams.Generics.Length * Params.MasterScale;
}

float USwordGen::GetGuardWidth()
{
	return Params.GuardParams.Generics.Width * Params.MasterScale;
}

float USwordGen::GetGuardThickness()
{
	return Params.GuardParams.Generics.Thickness * Params.MasterScale;
}

FPromeshMaterialParams USwordGen::GetGuardMaterialParams()
{
	return Params.GuardParams.Generics.MaterialParams;
}

/*
			Grip Parameters
*/

float USwordGen::GetGripLength()
{
	return Params.GripParams.Generics.Length * Params.MasterScale;
}

float USwordGen::GetGripWidth()
{
	return Params.GripParams.Generics.Width * Params.MasterScale;
}

float USwordGen::GetGripThickness()
{
	return Params.GripParams.Generics.Thickness * Params.MasterScale;
}

FPromeshMaterialParams USwordGen::GetGripMaterialParams()
{
	return Params.GripParams.Generics.MaterialParams;
}

/*
			Grip Cap Parameters
*/

float USwordGen::GetGripCapLength()
{
	return Params.GripCapParams.Generics.Length * Params.MasterScale;
}

float USwordGen::GetGripCapWidth()
{
	return Params.GripCapParams.Generics.Width * Params.MasterScale;
}

float USwordGen::GetGripCapThickness()
{
	return Params.GripCapParams.Generics.Thickness * Params.MasterScale;
}

FPromeshMaterialParams USwordGen::GetGripCapMaterialParams()
{
	return Params.GripCapParams.Generics.MaterialParams;
}