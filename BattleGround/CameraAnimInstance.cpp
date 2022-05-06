// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraAnimInstance.h"
#include "BattleGroundPlayer.h"
#include "BattleGroundPlayerController.h"
UCameraAnimInstance::UCameraAnimInstance()
{
}

void UCameraAnimInstance::NativeUpdateAnimation(float fDeltaTime)
{
	UpdateCharacterInfo();
}

void UCameraAnimInstance::UpdateCharacterInfo()
{
	if (ControllPawn != nullptr)
	{
		ABattleGroundPlayer * player = Cast<ABattleGroundPlayer>(ControllPawn);
		if (player != nullptr)
		{
			player->GetPlayerState(m_Stance, m_Movement, m_ViewMode, m_Gait, m_bisFire, m_bisAming);
		}
	}
}
