/*
	Created by Hoyeon Cho (Á¶ È£¿¬)
	Desc :Scene Capture actor used with ATifoGenerator.

	Requirements:
	 1.RenderTarget's Format must be set to RTF RGBA8 SRGB.
	 2.Scene Capture source must be set to "SceneColor(HDR) in RGB, Inv Opacity in A".
	 3.Using material instance for TIFO object needs Vector parameter for color of TIFO pixel.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "TifoRenderTarget.generated.h"

class USkeletalMeshComponent;

UCLASS()
class UE5PROGRAMMINGDEV_API ATifoRenderTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATifoRenderTarget();

protected:
	UPROPERTY(VisibleAnywhere)
	class USceneCaptureComponent2D* SceneCap = nullptr;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComp = nullptr;

public:
	FORCEINLINE USceneCaptureComponent2D* GetSceneCap() const { return SceneCap; };

	FORCEINLINE USkeletalMeshComponent* GetSkeletalMeshComp() const { return SkeletalMeshComp;};
};
