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
	//MakeDoubleSided();
	//Promesh->ClearAllMeshSections();
	
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

void UPromeshBuilder::MakeMirroredTri(FVector TriVerts[3], uint8 MirrorAxis)
{
	MakeTri(TriVerts);

	if (MirrorAxis & EMirrorAxis::X)
	{
		FVector M = { 1, -1, 1 };
		FVector MirroredVerts[3] = { TriVerts[0], TriVerts[2], TriVerts[1] };
		MirroredVerts[0] *= M;
		MirroredVerts[1] *= M;
		MirroredVerts[2] *= M;
		MakeMirroredTri(MirroredVerts, MirrorAxis & ~EMirrorAxis::X);
	}

	if (MirrorAxis & EMirrorAxis::Y)
	{
		FVector M = { -1, 1, 1 };
		FVector MirroredVerts[3] = { TriVerts[0], TriVerts[2], TriVerts[1] };
		MirroredVerts[0] *= M;
		MirroredVerts[1] *= M;
		MirroredVerts[2] *= M;
		MakeMirroredTri(MirroredVerts, MirrorAxis & ~EMirrorAxis::Y);
	}

	if (MirrorAxis & EMirrorAxis::Z)
	{
		FVector M = { 1, 1, -1 };
		FVector MirroredVerts[3] = { TriVerts[0], TriVerts[2], TriVerts[1] };
		MirroredVerts[0] *= M;
		MirroredVerts[1] *= M;
		MirroredVerts[2] *= M;
		MakeMirroredTri(MirroredVerts, MirrorAxis & ~EMirrorAxis::Z);
	}
}


// Move this into 
void UPromeshBuilder::StitchLines(UVertLine* L1, UVertLine* L2, bool bConnectEnds, uint8 MirrorAxis)
{
	UVertLine* Dense = L1->VertNum() >= L2->VertNum() ? L1 : L2;
	UVertLine* Sparse = L1->VertNum() >= L2->VertNum() ? L2 : L1;
	
	int32 Dn = Dense->VertNum();
	int32 Sn = Sparse->VertNum();

	int32 LastSi = 0;
	int32 Si = 0;
	int32 Di = 0;

	float Inc = (float)Sn / (float)Dn;

	int32 Stop = bConnectEnds ? Dn : Dn - 1;

	for (Di; Di < Stop; Di++)
	{
		Si = (int32)((float)Di * Inc);
		if (Si != LastSi)
		{
			FVector GapVerts[3] = { Sparse->At(Si - 1), Dense->At(Di), Sparse->At(Si) };
			MakeMirroredTri(GapVerts, MirrorAxis);
		}
		LastSi = Si;

		FVector TriVerts[3] = { Sparse->At(Si), Dense->At(Di), Dense->At(Di + 1) };
		MakeMirroredTri(TriVerts, MirrorAxis);
	}

	Si = (int32)((float)Di * Inc);
	if (Si != LastSi)
	{
		FVector GapVerts[3] = { Sparse->At(Si - 1), Dense->At(Di), Sparse->At(Si) };
		MakeMirroredTri(GapVerts, MirrorAxis);
	}
}

void UPromeshBuilder::StitchLineSequence(TArray<UVertLine*> Lines, bool bConnectEnds, uint8 MirrorAxis)
{
	for (int32 I = 0; I < Lines.Num() - 1; I++)
	{
		StitchLines(Lines[I], Lines[I + 1], bConnectEnds, MirrorAxis);
	}
}

void UPromeshBuilder::SetMaterialParams(FPromeshMaterialParams NewParams)
{
	MaterialParams = NewParams;
}

void UPromeshBuilder::MakeDoubleSided()
{
	bDoubleSided = true;
	int32 Num = Tris.Num();
	for (int32 I = 0; I < Num; I += 3)
	{
		int32 P1 = Tris[I + 2];
		int32 P2 = Tris[I + 1];
		int32 P3 = Tris[I];
		Tris.Append({ P1, P2, P3 });
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