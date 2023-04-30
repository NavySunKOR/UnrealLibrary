// Fill out your copyright notice in the Description page of Project Settings.


#include "Tifo/TifoGenerator.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Texture2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Rendering/Texture2DResource.h"

// Sets default values
ATifoGenerator::ATifoGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATifoGenerator::BeginPlay()
{
	Super::BeginPlay();
}



#if WITH_EDITORONLY_DATA
void ATifoGenerator::GenerateTifo()
{
	if (TexVisible != nullptr)
	{
		TexVisible->ReleaseResource();
		TexVisible->ConditionalBeginDestroy();
		TexVisible = nullptr;
	}
	USceneCaptureComponent2D* Cap2D = TifoRenderer->GetSceneCap();
	FTextureRenderTargetResource* TexResource = Cap2D->TextureTarget->GameThread_GetRenderTargetResource();
	TexVisible = UTexture2D::CreateTransient(TifoRenderer->GetSceneCap()->TextureTarget->SizeX, TifoRenderer->GetSceneCap()->TextureTarget->SizeY);
	
	TArray<FColor> ReadColors;
	TexResource->ReadPixels(ReadColors);

	void* data = TexVisible->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(data, ReadColors.GetData(), ReadColors.Num() * 4);
	TexVisible->GetPlatformData()->Mips[0].BulkData.Unlock();
	TexVisible->UpdateResource();


	DoneLoad();
}


void ATifoGenerator::DoneLoad()
{

	const FColor* FormatedImageData = static_cast<const FColor*>(TexVisible->GetPlatformData()->Mips[0].BulkData.LockReadOnly());

	UE_LOG(LogTemp, Warning, TEXT("Size : %d / %d"), TexVisible->GetSizeX(), TexVisible->GetSizeY())
		for (int32 X = 0; X < TexVisible->GetSizeX(); X++)
		{
			for (int32 Y = 0; Y < TexVisible->GetSizeY(); Y++)
			{
				FColor PixelColor = FormatedImageData[Y * TexVisible->GetSizeX() + X];
				uint8 Alpha = (PixelColor.A > 0) ? 0 : 1;

				//if Alpha is none, then don't draw
				if (Alpha == 0)
					continue;

				AStaticMeshActor* MeshActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
				MeshActor->GetStaticMeshComponent()->SetStaticMesh(UsingMesh);
				UMaterialInstanceDynamic* Dyna = UMaterialInstanceDynamic::Create(MatInst, this);
				Dyna->SetVectorParameterValue(FName("Color"), FVector4((float)PixelColor.R / 255.f, (float)PixelColor.G/255.f , (float)PixelColor.B / 255.f, 1));
				MeshActor->GetStaticMeshComponent()->SetMaterial(0, Dyna);
				//NewLoc
				FVector Loc = MeshActor->GetActorLocation();
				Loc.X += X * UNIT_NEXTPIXEL;
				Loc.Y += Y * UNIT_NEXTPIXEL;

				MeshActor->SetActorLocation(Loc);
				MeshActor->SetActorScale3D(UNIT_PIXELSCALE);

				RenderActors.Add(MeshActor);
			}
		}

	TexVisible->GetPlatformData()->Mips[0].BulkData.Unlock();
}
#endif