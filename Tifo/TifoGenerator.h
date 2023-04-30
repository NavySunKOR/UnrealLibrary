/*
	Created by Hoyeon Cho (Á¶ È£¿¬)
	Desc :Generating TIFO in sports .
	 It generates TIFO object based on Scene Captured Texture in ATifoRenderTarget.

	Requirements:
	 1.RenderTarget's Format must be set to RTF RGBA8 SRGB.
	 2.Scene Capture source must be set to "SceneColor(HDR) in RGB, Inv Opacity in A".
	 3.Using material instance for TIFO object needs Vector parameter for color of TIFO pixel.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tifo/TifoRenderTarget.h"
#include "TifoGenerator.generated.h"

#define UNIT_NEXTPIXEL 10.f
#define UNIT_PIXELSCALE FVector(0.1f,0.1f,0.1f)

UCLASS()
class UE5PROGRAMMINGDEV_API ATifoGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATifoGenerator();

protected:
	UPROPERTY(EditAnywhere)
	class UMaterialInstance* MatInst;


	UPROPERTY(EditAnywhere)
	class UStaticMesh* UsingMesh;

	UPROPERTY()
	TArray<class AStaticMeshActor*> RenderActors;


	UPROPERTY(EditAnywhere)
	ATifoRenderTarget* TifoRenderer;

	UPROPERTY(VisibleAnywhere)
	UTexture2D* TexVisible;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	UFUNCTION(CallInEditor)
	void GenerateTifo();

	UFUNCTION()
	void DoneLoad();
#endif
};
