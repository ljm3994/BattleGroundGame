// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueZone.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerWidget.h"
#include "BattleGroundPlayer.h"
// Sets default values
ABlueZone::ABlueZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateEditorOnlyDefaultSubobject<USceneComponent>(TEXT("DefaultSceneBase")));
	mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>("Mesh");

	if (mesh)
	{
		mesh->AlwaysLoadOnClient = true;
		mesh->AlwaysLoadOnServer = true;
		mesh->bOwnerNoSee = false;
		mesh->bCastDynamicShadow = true;
		mesh->bAffectDynamicIndirectLighting = true;
		mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		mesh->SetCollisionProfileName(MeshCollisionProfileName);
		mesh->SetGenerateOverlapEvents(false);
		mesh->SetCanEverAffectNavigation(false);
		mesh->SetupAttachment(GetRootComponent());
	}
	m_TimeBulerZone = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimerBlue"));
	m_TimeLineUpdateEvent.BindUFunction(this, TEXT("UpdateCircle"));
	m_TimeLineFinisihEvent.BindUFunction(this, TEXT("FinishCircle"));
}

// Called when the game starts or when spawned
void ABlueZone::BeginPlay()
{
	Super::BeginPlay();
	
	mesh->SetGenerateOverlapEvents(true);

	m_TimeBulerZone->AddInterpFloat(m_TimeBulerZoneCurve, m_TimeLineUpdateEvent);
	m_TimeBulerZone->SetTimelineFinishedFunc(m_TimeLineFinisihEvent);

	mesh->OnComponentBeginOverlap.__Internal_AddDynamic(this, &ABlueZone::OnCircleCollisionBeginOverlap, TEXT("OnCircleCollisionBeginOverlap"));
	mesh->OnComponentEndOverlap.__Internal_AddDynamic(this, &ABlueZone::OnCircleCollisionEndOverlap, TEXT("OnCircleCollisionEndOverlap"));
}

// Called every frame
void ABlueZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlueZone::StageStart_Implementation()
{
	SetCircle();
}

void ABlueZone::SetCircle()
{
	m_vecOldLocation = GetActorLocation();
	float XPos = UKismetMathLibrary::RandomFloatInRange(-3000.0, 4900.0);
	float YPos = UKismetMathLibrary::RandomFloatInRange(-3000.0, 4600.0);

	m_vecNewLocation = UKismetMathLibrary::MakeVector(XPos, YPos, 0.0f) + m_vecOldLocation;
	m_vecScale = mesh->GetComponentScale();
}

void ABlueZone::UpdateCircle(float fAlpha)
{
	FVector vecB = m_vecScale - m_fShrinkAmount;
	FVector Scale = UKismetMathLibrary::VLerp(m_vecScale, vecB, fAlpha);

	mesh->SetWorldScale3D(Scale);

	FVector Loc = UKismetMathLibrary::VLerp(m_vecOldLocation, m_vecNewLocation, fAlpha);

	SetActorLocation(Loc);
}

void ABlueZone::FinishCircle()
{
	m_Timer->StartTimer();
}

void ABlueZone::OnCircleCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABattleGroundPlayer* player = Cast<ABattleGroundPlayer>(OtherActor);

	if (player != nullptr)
	{
		player->m_bIsIncir = true;
	}
}

void ABlueZone::OnCircleCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABattleGroundPlayer* player = Cast<ABattleGroundPlayer>(OtherActor);

	if (player != nullptr)
	{
		player->m_bIsIncir = false;
	}
}

