// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BaseWeapone.generated.h"

/**
 * 
 */
UCLASS(config = Game, BlueprintType)
class BATTLEGROUND_API ABaseWeapone : public ABaseItem
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		bool bisInit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		float m_fBulletSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		float m_fBulletSpread;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		class ABattleGroundPlayer* m_OwnerCharacter;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		FWeaponData m_WeaponData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		EFireCategory m_fireCategory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* m_FireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		USoundBase* m_FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		USoundBase* m_DrySound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		bool m_bisFire;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* mesh;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* Cap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		FName m_MuzzleBoneName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paticle", meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* m_ParticleShell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paticle", meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* m_ParticleTrail;
	ABaseWeapone(const FObjectInitializer& ObjectInitializer);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float GetBulletSpeed() { return m_fBulletSpeed; }
	void SetOwner(ABattleGroundPlayer* m_Character);
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		bool HaveAmmoInWeapon(bool& isAmmoFull);
	UFUNCTION(BlueprintNativeEvent)
		void StartFire();
	virtual void StartFire_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void StopFire();
	virtual void StopFire_Implementation();

	virtual void RifleFire();
	void BulletBuild();
	void IncreaseBulletSpread(float fIncreaseAmount);
	void IncreaseCameraVertical();
};
