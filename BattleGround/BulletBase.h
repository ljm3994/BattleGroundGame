// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletBase.generated.h"

UCLASS()
class BATTLEGROUND_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABulletBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
		void OnBulletHit(FHitResult reslut);

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* MainMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Particle)
		class UParticleSystemComponent* TrailParticle;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
		class UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
		class UParticleSystem* BloodParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
		class UDecalComponent* Decal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		FVector Velocity = FVector(100.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float BulletDestroyTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
		bool isBulletHitDebug = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
		bool isBulletLineDebug = true;

	float BulletExpiry = 0.0f;

};
