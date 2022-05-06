// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerStateEnum.h"
#include "CameraAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API UCameraAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	PlayerStance m_Stance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	PlayerMoveState m_Movement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	PlayerViewMode m_ViewMode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	PlayerGait m_Gait;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	bool m_bisFire;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	bool m_bisAming;
public:
	class APawn* ControllPawn;
	UCameraAnimInstance();

	virtual void NativeUpdateAnimation(float fDeltaTime) override;
	void UpdateCharacterInfo();
};
