// Fill out your copyright notice in the Description page of Project Settings.


#include "RecordingActor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

ARecordingActor::ARecordingActor(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("MainScene"));
	mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>("MainMesh");
	m_Cam = CreateDefaultSubobject<UCameraComponent>("MainCam");
	m_CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>("MainCapture");
	mesh->SetupAttachment(GetRootComponent());
	m_CaptureComponent->SetupAttachment(m_Cam);

	m_bisRotation = false;
	m_vecStartLoc = FVector2D(0.0f, 0.0f);
}

// Called when the game starts or when spawned
void ARecordingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARecordingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARecordingActor::StartClick_Implementation(FVector2D Vec)
{
	m_bisRotation = true;
	m_vecStartLoc = Vec;
}

void ARecordingActor::EndClick_Implementation()
{
	m_bisRotation = false;
}

void ARecordingActor::MouseMove_Implementation(FVector2D Vec)
{
	if (m_bisRotation)
	{
		FVector2D MoveVec = m_vecStartLoc - Vec;

		mesh->AddWorldRotation(UKismetMathLibrary::MakeRotator(0.0f, 0.0f, MoveVec.X * -1.f));

		m_vecStartLoc = Vec;
	}
}

