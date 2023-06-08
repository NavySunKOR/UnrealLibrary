// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGenerator/NoiseMapGenerator.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ANoiseMapGenerator::ANoiseMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SizeX = 256;
	SizeY = 256;

	Scale = 1.f;
	Persistence = 0.5f;
	Octave = 8;
	OffsetY = 0;
	OffsetX = 0;

}

// Called when the game starts or when spawned
void ANoiseMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANoiseMapGenerator::GenerateNoiseMap()
{
	GenerateNoiseData();
	GenerateTexture();

}


void ANoiseMapGenerator::GenerateNoiseData()
{
	if (NoiseData)
	{
		delete NoiseData;
		NoiseData = nullptr;
	}

	NoiseData = new float[SizeX * SizeY];

	for (int32 YPos = 0; YPos < SizeY; ++YPos)
	{
		for (int32 XPos = 0; XPos < SizeX; ++XPos)
		{
			float AccFrequency = Scale;
			float AccAmplitude = 1;
			float MaxValue = 0;
			float Sumup = 0;
			for (int32 Octa = 0; Octa < Octave; ++Octa)
			{
				float XPosF = ((float)XPos / (float)SizeX + OffsetX) * AccFrequency;
				float YPosF = ((float)YPos / (float)SizeY + OffsetY) * AccFrequency;

				Sumup += FMath::PerlinNoise2D(FVector2D(XPosF, YPosF)) * AccAmplitude;

				MaxValue += AccAmplitude;
				AccAmplitude *= Persistence;
				AccFrequency *= 2;
			}

			float Result = Sumup / MaxValue;
			Result = (Result + 1.f) / 2.f;
			Result = FMath::Clamp(Result, 0.f, 1.f);

			NoiseData[XPos + YPos * SizeX] = Result;
		}
	}

}


void ANoiseMapGenerator::GenerateTexture()
{
	if (GeneratedTexture == nullptr)
	{
		GeneratedTexture = UTexture2D::CreateTransient(SizeX, SizeY, PF_B8G8R8A8);
	}


	FColor* Color = (FColor*)(GeneratedTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
	for (int32 YPos = 0; YPos < SizeY; ++YPos)
	{
		for (int32 XPos = 0; XPos < SizeX; ++XPos)
		{
			int32 Idx = XPos + YPos * SizeX;
			Color[Idx].R = NoiseData[Idx] * 255;
			Color[Idx].G = NoiseData[Idx] * 255;
			Color[Idx].B = NoiseData[Idx] * 255;
			Color[Idx].A = 255;
		}
	}

	GeneratedTexture->PlatformData->Mips[0].BulkData.Unlock();
	GeneratedTexture->UpdateResource();
}