// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBase.h"
#include "Engine.h"
#include "Engine\Classes\Particles/ParticleSystem.h"
#include "Engine\Classes\Particles/ParticleSystemComponent.h"

// Sets default values
ABulletBase::ABulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MainMesh = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("BulletMesh"));
	MainMesh->SetOnlyOwnerSee(true);
	MainMesh->SetupAttachment(GetRootComponent());
	MainMesh->bCastDynamicShadow = false;
	MainMesh->CastShadow = false;
	MainMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	MainMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticle"));
	TrailParticle->SetupAttachment(MainMesh);
	TrailParticle->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	TrailParticle->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FHitResult Hitresult;

	FVector StartTrace = this->GetActorLocation();
	FVector EndTrace = (Velocity * DeltaTime) + StartTrace;

	EndTrace.Z += this->GetActorRotation().Pitch;

	FCollisionQueryParams CollisionParam;
	CollisionParam.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Hitresult, StartTrace, EndTrace, ECC_Destructible, CollisionParam))
	{
		if (Hitresult.GetActor())
		{
			if (isBulletHitDebug)
			{
				DrawDebugSolidBox(GetWorld(), Hitresult.ImpactPoint, FVector(10.0f), FColor::Blue, true);
			}

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Hitresult.ImpactPoint);

			OnBulletHit(Hitresult);
		}

		Destroy();
	}
	else
	{
		BulletExpiry += DeltaTime;

		if (isBulletLineDebug)
		{
			DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(0.0f, -BulletExpiry * 80.0f, 100.0f), true);
		}

		SetActorLocation(EndTrace);

		Velocity += FVector(0.0f, 0.0f, -200.0f) * DeltaTime;
	}

	if (BulletExpiry > BulletDestroyTime)
	{
		Destroy();
	}
}

