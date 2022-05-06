// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapone.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BulletBase.h"
#include "BattleGroundPlayer.h"
#include "Camera/CameraComponent.h"
#include "UObject/UnrealType.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ABaseWeapone::ABaseWeapone(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    m_OwnerCharacter = NULL;
	SetRootComponent(CreateEditorOnlyDefaultSubobject<USceneComponent>(TEXT("DefaultSceneBase")));
	//meshProperty = CreateDefaultSubobject<FProperty>(TEXT("Property"));
	Cap = CreateOptionalDefaultSubobject<USphereComponent>("Capsule");
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

	Cap->SetupAttachment(mesh);
}

void ABaseWeapone::BeginPlay()
{
	Super::BeginPlay();

	
	if (m_bisinitdata && m_ItemInfo.m_Category == EItemCategory::Item_Weapon)
	{
		UDataTable* DT_WeaponData = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Data/DT_WeaponData.DT_WeaponData'"));

		FWeaponData* temp = DT_WeaponData->FindRow<FWeaponData>(m_ItemInfo.m_ItemName, FString(""));
		memcpy(&m_WeaponData, temp, sizeof(FWeaponData));
	}
}

void ABaseWeapone::SetOwner(ABattleGroundPlayer* m_Character)
{
	m_OwnerCharacter = m_Character;
}

bool ABaseWeapone::HaveAmmoInWeapon(bool& isAmmoFull)
{
	int AmmoCnt = m_WeaponData.m_iAmmoCount;
	int AmmoMaxCnt = m_WeaponData.m_iAmmoMax;
	bool bisReturn = false;

	if (AmmoCnt > 0)
		bisReturn = true;

	if (AmmoCnt == AmmoMaxCnt)
		isAmmoFull = true;
	else
		isAmmoFull = false;

	return bisReturn;
}

void ABaseWeapone::StartFire_Implementation()
{
	m_bisFire = true;
	RifleFire();

	if (m_fireCategory == EFireCategory::Fire_Auto)
	{
		UKismetSystemLibrary::K2_SetTimer(this, TEXT("RifleFire"), m_WeaponData.m_fWeaponDelayFire, true);
	}
	else
	{
		StopFire();
	}
}

void ABaseWeapone::StopFire_Implementation()
{
	m_bisFire = false;
}

void ABaseWeapone::RifleFire()
{
	if (m_bisFire)
	{
		bool isFull = false;
		if (HaveAmmoInWeapon(isFull)) // 현재 무기의 총알이 들어있는지 확인한다.
		{
			m_WeaponData.m_iAmmoCount--;       //해당 무기의 총알 갯수 감소
			m_OwnerCharacter->PlayAnimMontage(m_FireMontage);

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_FireSound, GetActorLocation());
			FTransform trans = mesh->GetSocketTransform(TEXT("AmmoShell"));         
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_ParticleShell, UKismetMathLibrary::MakeTransform(trans.GetLocation(), trans.GetRotation().Rotator(), FVector(1.0f, 1.0f, 1.0f)), true);
			BulletBuild();          //총알을 생성한다.

			if (m_ParticleTrail != nullptr)        //총알의 Trail를 생성한다
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_ParticleTrail,
					UKismetMathLibrary::MakeTransform(
						mesh->GetSocketTransform(m_MuzzleBoneName).GetLocation(), FRotator(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f)), true)
					->SetVectorParameter(TEXT("ShockBeamEnd"), FVector(0.0f, 0.0f, 0.0f));
			}

			IncreaseBulletSpread(0.1f);       //탄퍼짐을 증가시킨다.
			IncreaseCameraVertical();        //반동을 표현하기 위해 카메라를 흔들리게 한다.
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_DrySound, GetActorLocation());
		}
	}
}

void ABaseWeapone::BulletBuild()
{
	UCameraComponent* cam = m_OwnerCharacter->GetCamer();       
	FVector Loc = cam->GetComponentLocation();                       //월드의 위치를 가져온다
	FVector SocketLoc = mesh->GetSocketTransform(m_MuzzleBoneName).GetLocation();                  
	FRotator BulletRot = cam->GetComponentRotation();                           //월드 회전값을 가져온다
	FVector BulletDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(UKismetMathLibrary::GetForwardVector(BulletRot), m_fBulletSpeed) * 50000.0f;
	FHitResult OutResult;
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), Loc, Loc + BulletDir, ETraceTypeQuery::TraceTypeQuery1, false,
		TArray<AActor*>(), EDrawDebugTrace::Type::ForDuration, OutResult, true))           //디버깅을 위해 표현해야지
	{
		BulletRot = UKismetMathLibrary::FindLookAtRotation(SocketLoc, OutResult.Location);
	}

	FActorSpawnParameters spawnParameters;
	spawnParameters = FActorSpawnParameters();
	spawnParameters.Owner = m_OwnerCharacter;

	GetWorld()->SpawnActor<ABulletBase>(ABulletBase::StaticClass(), UKismetMathLibrary::MakeTransform(SocketLoc, BulletRot, FVector(1.0f, 1.0f, 1.0f)), spawnParameters);
}

void ABaseWeapone::IncreaseBulletSpread(float fIncreaseAmount)
{
	if (m_fBulletSpread + fIncreaseAmount >= m_WeaponData.m_VerticalRebound.m_fMaxValue)
	{
		m_fBulletSpread = m_WeaponData.m_VerticalRebound.m_fMaxValue;
	}
	else
	{
		m_fBulletSpread += fIncreaseAmount;
	}
}

void ABaseWeapone::IncreaseCameraVertical()
{
	m_OwnerCharacter->CameraVerticalIncrease(m_WeaponData.m_VerticalRebound);
}
