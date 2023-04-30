// Fill out your copyright notice in the Description page of Project Settings.


#include "Tifo/TifoRenderTarget.h"

// Sets default values
ATifoRenderTarget::ATifoRenderTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneCap = CreateDefaultSubobject<USceneCaptureComponent2D>(FName("SceneCap"));

	SceneCap->SetupAttachment(RootComponent);
	SceneCap->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCap->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent >(FName("Skel"));
	SkeletalMeshComp->SetupAttachment(RootComponent);


}


