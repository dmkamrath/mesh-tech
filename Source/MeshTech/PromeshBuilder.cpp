// Fill out your copyright notice in the Description page of Project Settings.


#include "PromeshBuilder.h"

UPromeshBuilder::UPromeshBuilder()
{
	SameVertThresh = 0.0001;
	bDoubleSided = false;
}

void UPromeshBuilder::BuildMesh(UProceduralMeshComponent* Promesh, int32 SectionIndex)
{
	//if (GetNormals().Num() != Tris.Num() / 3)
	//	return;
	MakeDoubleSided();
	//Promesh->ClearAllMeshSections();
	//RecalculateNormals();
	
	Promesh->CreateMeshSection(SectionIndex, GetVerts(), GetTris(), GetNormals(), GetUvs(), GetVertexColors(), GetTangents(), true);
	ApplyMaterialParams(Promesh, SectionIndex);
	Reset();
}

void UPromeshBuilder::ApplyMaterialParams(UProceduralMeshComponent* Promesh, int32 SectionIndex)
{
	UMaterialInterface* Mat = GetMaterialBase();
	UMaterialInstanceDynamic* Dmat = UMaterialInstanceDynamic::Create(Mat, this);
	MaterialParams.Apply(Dmat);
	Promesh->SetMaterial(SectionIndex, Dmat);
}

void UPromeshBuilder::Reset()
{
	Verts.Empty();
	VertNormals.Empty();
	Tris.Empty();
	VertexColors.Empty();
}

int32 UPromeshBuilder::AddVert(FVector Vert)
{
	if (HasVert(Vert))
		return -1;
	VertexColors.Add(MaterialParams.Color);
	return ExpressAddVert(Vert);
}

bool UPromeshBuilder::HasVert(FVector Vert)
{
	if (MaterialParams.bShadeFlat)
		return false;

	for (auto V : Verts)
		if (V.Equals(Vert, SameVertThresh))
			return true;
	return false;
}

int32 UPromeshBuilder::ExpressAddVert(FVector Vert)
{
	VertNormals.Add(FVertexNormal());
	return Verts.Add(Vert);
}

int32 UPromeshBuilder::GetVertIndex(FVector Vert)
{
	int32 I = AddVert(Vert);
	if (I > -1)
		return I;

	for (I = 0; I < Verts.Num(); I++)
		if (Verts[I].Equals(Vert, SameVertThresh))
			return I;
	return -1;
}

void UPromeshBuilder::MakeQuad(FVector QuadVerts[4])
{
	FVector T1Verts[3] = { QuadVerts[0], QuadVerts[1], QuadVerts[2] };
	FVector T2Verts[3] = { QuadVerts[0], QuadVerts[2], QuadVerts[3] };

	// Or 0 1 3 and 1 2 3 if 0 -> 3 is shorter than 0 -> 2

	MakeTri(T1Verts);
	MakeTri(T2Verts);
}

void UPromeshBuilder::MakeMirroredQuad(FVector QuadVerts[4], uint8 MirrorAxis)
{
	FVector T1Verts[3] = { QuadVerts[0], QuadVerts[1], QuadVerts[2] };
	FVector T2Verts[3] = { QuadVerts[0], QuadVerts[2], QuadVerts[3] };
	MakeMirroredTri(T1Verts, MirrorAxis);
	MakeMirroredTri(T2Verts, MirrorAxis);
}

void UPromeshBuilder::MakeTri(FTri Tri)
{
	FVector TriVerts[3] = { Tri.P0, Tri.P1, Tri.P2 };
	MakeTri(TriVerts);
}

void UPromeshBuilder::MakeTri(FVector TriVerts[3])
{
	FVector TriNormal = GetTriNormal(TriVerts);

	int32 Iv1 = GetVertIndex(TriVerts[0]);
	int32 Iv2 = GetVertIndex(TriVerts[1]);
	int32 Iv3 = GetVertIndex(TriVerts[2]);

	VertNormals[Iv1].AddFaceNormal(TriNormal);
	VertNormals[Iv2].AddFaceNormal(TriNormal);
	VertNormals[Iv3].AddFaceNormal(TriNormal);

	Tris.Add(Iv1);
	Tris.Add(Iv2);
	Tris.Add(Iv3);
}

void UPromeshBuilder::MakeMirroredTri(FTri Tri, uint8 MirrorAxis)
{
	FVector TriVerts[3] = { Tri.P0, Tri.P1, Tri.P2 };
	MakeMirroredTri(TriVerts, MirrorAxis);
}

void UPromeshBuilder::MakeMirroredTri(FVector TriVerts[3], uint8 MirrorAxis)
{
	FTri Tri(TriVerts[0], TriVerts[1], TriVerts[2]);
	MakeTri(Tri);


	if (MirrorAxis & EMirrorAxis::X)
	{
		MakeMirroredTri(Tri.Mirror({ 1, -1, 1 }, false), MirrorAxis & ~EMirrorAxis::X);
	}

	if (MirrorAxis & EMirrorAxis::Y)
	{
		MakeMirroredTri(Tri.Mirror({ -1, 1, 1 }, false), MirrorAxis & ~EMirrorAxis::Y);
	}

	if (MirrorAxis & EMirrorAxis::Z)
	{
		MakeMirroredTri(Tri.Mirror({ 1, 1, -1 }, false), MirrorAxis & ~EMirrorAxis::Z);
	}
}

void UPromeshBuilder::StitchMirroredLines(UVertLine* L, uint8 MirrorAxis)
{
	if (MirrorAxis & EMirrorAxis::X)
	{
		UVertLine* MirroredLine = L->GetMirroredLine({ 1, -1, 1 });
		StitchLines(L, MirroredLine, true);
		StitchMirroredLines(MirroredLine, MirrorAxis & ~EMirrorAxis::X);
	}

	if (MirrorAxis & EMirrorAxis::Y)
	{
		UVertLine* MirroredLine = L->GetMirroredLine({ -1, 1, 1 });
		StitchLines(L, MirroredLine, true);
		StitchMirroredLines(MirroredLine, MirrorAxis & ~EMirrorAxis::Y);
	}

	if (MirrorAxis & EMirrorAxis::Z)
	{
		UVertLine* MirroredLine = L->GetMirroredLine({ 1, 1, -1 });
		StitchLines(L, MirroredLine, true);
		StitchMirroredLines(MirroredLine, MirrorAxis & ~EMirrorAxis::Z);
	}
}


void UPromeshBuilder::StitchLines(UVertLine* L1, UVertLine* L2, bool bConnectEnds, uint8 MirrorAxis)
{
	UVertLine* Dense = L1->VertNum() >= L2->VertNum() ? L1 : L2;
	UVertLine* Sparse = L1->VertNum() >= L2->VertNum() ? L2 : L1;

	// Try to clamp verts before mirroring

	if (MirrorAxis & EMirrorAxis::X)
	{
		Dense->ClampVertsForMirrorAxis({ 1, -1, 1 });
		Sparse->ClampVertsForMirrorAxis({ 1, -1, 1 });
	}

	if (MirrorAxis & EMirrorAxis::Y)
	{
		Dense->ClampVertsForMirrorAxis({ -1, 1, 1 });
		Sparse->ClampVertsForMirrorAxis({ -1, 1, 1 });
	}

	if (MirrorAxis & EMirrorAxis::Z)
	{
		Dense->ClampVertsForMirrorAxis({ 1, 1, -1 });
		Sparse->ClampVertsForMirrorAxis({ 1, 1, -1 });
	}

	bool bJagged = false;
	
	int32 Dn = Dense->VertNum();
	int32 Sn = Sparse->VertNum();

	bool bInvert = Dn != Sn && (Dn - Sn) % 2 == 0;

	int32 LastSi = 0;
	int32 Si = 0;
	int32 Di = 0;

	float Inc = (float)Sn / (float)Dn;

	int32 Stop = bConnectEnds ? Dn : Dn - 1;

	// sometimes there may be a different arrangement of triangles that minimizes distance
	// and will look better

	for (Di; Di < Stop; Di++)
	{
		Si = (int32)((float)Di * Inc);


		if (Si != LastSi) // stitching a quad (two tris)
		{
			// Verts for quad are Si, Si - 1, Di, Di + 1


			FVector TL = Sparse->At(Si - 1);
			FVector TR = Sparse->At(Si);
			FVector BL = Dense->At(Di);
			FVector BR = Dense->At(Di + 1);

			float D1 = (BL - BL).SizeSquared();
			float D2 = (TR - BR).SizeSquared();

			// Not sure this is needed, but works if you want it to be jagged!
			bool bCutUp = D1 < D2 && !bJagged;

			if (bCutUp) // arrangement 1: quad with up diagonal
			{
				FTri T1(TL, BL, TR);
				MakeMirroredTri(T1, MirrorAxis);

				FTri T2(TR, BL, BR);
				MakeMirroredTri(T2, MirrorAxis);
			}
			else // arrangement 2: quad with down diagonal
			{
				FTri T1(TL, BL, BR);
				MakeMirroredTri(T1, MirrorAxis);

				FTri T2(TR, TL, BR);
				MakeMirroredTri(T2, MirrorAxis);
			}
		}
		else // stitching a tri
		{
			FTri T(Sparse->At(Si), Dense->At(Di), Dense->At(Di + 1));
			MakeMirroredTri(T, MirrorAxis);
		}

		LastSi = Si;
	}

	Si = (int32)((float)Di * Inc);
	if (Si != LastSi)
	{
		FTri T(Sparse->At(Si - 1), Dense->At(Di), Sparse->At(Si));
		MakeMirroredTri(T, MirrorAxis);
	}
}

void UPromeshBuilder::StitchLineSequence(TArray<UVertLine*> Lines, bool bConnectEnds, uint8 MirrorAxis)
{
	for (int32 I = 0; I < Lines.Num() - 1; I++)
	{
		StitchLines(Lines[I], Lines[I + 1], bConnectEnds, MirrorAxis);
	}
}

void UPromeshBuilder::AddBorderLines(UVertLine* CenterLine, float SegmentThickness, int32 SegmentVertCount, uint8 MirrorAxis, float ThicknessDecay)
{
	TArray<UVertLine*> Enc = CenterLine->BorderLine(SegmentThickness, SegmentVertCount, ThicknessDecay);

	//Doesnt work too good
	//StitchMirroredLines(Enc[0], EMirrorAxis::X);

	for (int32 I = 1; I < Enc.Num(); I++)
	{
		StitchLines(Enc[I], Enc[I - 1], true, MirrorAxis);
	}

	// Cap end
	UVertLine* Last = Enc[Enc.Num() - 1];
	StitchLines(Last, Last->GetCenterPointAsLine(), true, MirrorAxis);

	// Cap start
	UVertLine* First = Enc[0];
	StitchLines(First, First->GetCenterPointAsLine(), true, MirrorAxis);
}

void UPromeshBuilder::AddDecoBorderLines(UVertLine* CenterLine, float SegmentThickness, int32 SegmentVertCount, FPromeshMaterialParams DecoMaterialParams, uint8 MirrorAxis, float ThicknessDecay)
{
	FPromeshMaterialParams CachedMaterialParams = MaterialParams;
	SetMaterialParams(DecoMaterialParams);
	AddBorderLines(CenterLine, SegmentThickness, SegmentVertCount, MirrorAxis, ThicknessDecay);
}

void UPromeshBuilder::SetMaterialParams(FPromeshMaterialParams NewParams)
{
	MaterialParams = NewParams;
}

void UPromeshBuilder::MakeDoubleSided()
{
	bDoubleSided = true;
	int32 Num = Tris.Num();
	TArray<FTri> OtherSideTris;
	for (int32 I = 0; I < Num; I += 3)
	{
		int32 P1 = Tris[I + 2];
		int32 P2 = Tris[I + 1];
		int32 P3 = Tris[I];
		FTri T(Verts[P1], Verts[P2], Verts[P3]);
		OtherSideTris.Add(T);
	}
	for (auto T : OtherSideTris)
	{
		MakeTri(T);
	}
}

bool UPromeshBuilder::IsTriangleClockwise(FTri Tri)
{
	float A = Tri.P1.X - Tri.P0.X * Tri.P1.Y - Tri.P0.Y * Tri.P1.Z - Tri.P0.Z;
	float B = Tri.P2.X - Tri.P1.X * Tri.P2.Y - Tri.P1.Y * Tri.P2.Z - Tri.P1.Z;
	float C = Tri.P0.X - Tri.P2.X * Tri.P0.Y - Tri.P2.Y * Tri.P0.Z - Tri.P2.Z;
	float Sum = A + B + C;
	return Sum > 0.0;
}

void UPromeshBuilder::RecalculateNormals()
{
	FVector LastNormal(0, 0, 0);
	for (int32 I = 0; I < Tris.Num(); I += 3)
	{
		int32 V1I = Tris[I];
		int32 V2I = Tris[I + 1];
		int32 V3I = Tris[I + 2];
		FTri Tri(Verts[V1I], Verts[V2I], Verts[V3I]);

		FVector ThisNormal = Tri.GetNormal();

		bool bFlip = false;

		if (LastNormal != FVector(0, 0, 0))
		{
			float Dp = FVector::DotProduct(ThisNormal, LastNormal);
			bFlip = Dp <= -0.4;
		}
		
		//bool Cw = IsTriangleClockwise(FTri(Verts[V1I], Verts[V2I], Verts[V3I]));
		if (bFlip)
		{
			Tris[I] = V3I;
			Tris[I + 2] = V1I;
			Tri.Invert();
		}
		LastNormal = Tri.GetNormal();
	}
}

TArray<FVector> UPromeshBuilder::GetVerts()
{
	return Verts;
}

TArray<int32> UPromeshBuilder::GetTris()
{
	return Tris;
}

TArray<FVector> UPromeshBuilder::GetNormals()
{
	TArray<FVector> Normals;
	for (auto Vn : VertNormals)
		Normals.Add(Vn.Get());
	return Normals;
}

FVector UPromeshBuilder::GetTriNormal(FVector TriVerts[3])
{
	FVector A = TriVerts[2] - TriVerts[0];
	FVector B = TriVerts[1] - TriVerts[0];
	return FVector::CrossProduct(A, B).GetSafeNormal();
}

TArray<FVector2D> UPromeshBuilder::GetUvs()
{
	return Uvs;
}

TArray<FColor> UPromeshBuilder::GetVertexColors()
{
	return VertexColors;
}

TArray<FProcMeshTangent> UPromeshBuilder::GetTangents()
{
	TArray<FProcMeshTangent> Tangents;
	int32 Num = Tris.Num();
	for (int32 I = 0; I < Num; I++)
	{
		//FVector SurfaceTangent = (Verts[Tris[I]] - Verts[Tris[I + 1]]).GetSafeNormal();
		FVector SurfaceTangent = FVector(1, 1, 0);
		Tangents.Add(FProcMeshTangent(SurfaceTangent, true));
	}
	return Tangents;
}

UMaterialInterface* UPromeshBuilder::GetMaterialBase()
{
	return MaterialBase;
}