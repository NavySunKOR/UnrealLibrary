// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoiseMapGenerator.generated.h"

UCLASS()
class UE5PROGRAMMINGDEV_API ANoiseMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoiseMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Noise Map Setting")
	int32 SizeX;

	UPROPERTY(EditAnywhere, Category = "Noise Map Setting")
	int32 SizeY;

	UPROPERTY(EditAnywhere, Category = "Noise Map Setting")
	int32 OffsetX;

	UPROPERTY(EditAnywhere, Category = "Noise Map Setting")
	int32 OffsetY;

	UPROPERTY(EditAnywhere, Category = "Noise Map Setting")
	int32 Octave;

	UPROPERTY(EditAnywhere, Category = "Noise Map Setting")
	float Persistence;

	//UPROPERTY(EditAnywhere, Category = "Noise Map Setting")
	//float Lacunarity;

	UPROPERTY(EditAnywhere, Category = "Noise Map Setting")
	float Scale;



	UPROPERTY(VisibleAnywhere)
	UTexture2D* GeneratedTexture = nullptr;


#if WITH_EDITOR 

	UFUNCTION(CallInEditor)
	void GenerateNoiseMap();

#endif

protected: 
	//Range∏¶ 0~1∑Œ InverseLerp «ÿ¡‹.
	float* NoiseData = nullptr;
	void GenerateNoiseData();

	void GenerateTexture();



};
