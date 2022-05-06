// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BattleGroundPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerStateEnum.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
}

void UPlayerAnimInstance::NativeUpdateAnimation(float fDeltaTime)
{
	Super::NativeUpdateAnimation(fDeltaTime);
	
	AnimStart();
}

void UPlayerAnimInstance::AnimStart_Implementation()
{
	APawn * pawn = TryGetPawnOwner();

	if (IsValid(pawn))
	{
		ABattleGroundPlayer* player = Cast<ABattleGroundPlayer>(pawn);

		if (pawn != nullptr)
		{
			m_player = player;

			FRotator Actorrot = player->GetActorRotation();
			FRotator ConRot = player->GetControlRotation();

			SpreadDir(Actorrot);
			CalculateYawPitch(Actorrot, ConRot);
			MovePlayer();
			CrounchPlayer();
			PlayerStateSet();
			m_bisLieProne = (m_player->GetPlayterStance() == PlayerStance::Stance_Prone);
			m_bisFire = m_player->GetIsFire();
		}
	}
}

void UPlayerAnimInstance::SpreadDir(FRotator rot)
{
	m_fSpeed = m_player->GetVelocity().Size();

	if (m_fSpeed <= 1.0f)
	{
		m_fDirection = 0.0f;
	}
	else
	{
		m_fDirection = CalculateDirection(m_player->GetVelocity(), rot);
	}
	
}

void UPlayerAnimInstance::CalculateYawPitch(FRotator rot, FRotator rot2)
{
	FRotator Normal = UKismetMathLibrary::NormalizedDeltaRotator(rot, rot2);
	float TargetPitch = UKismetMathLibrary::ClampAngle(Normal.Pitch, -89.0f, 89.0f);
	float TargetYaw = UKismetMathLibrary::ClampAngle(Normal.Yaw, -89.0f, 89.0f);
	FRotator TargetRot = UKismetMathLibrary::MakeRotator(0.0f, TargetPitch * -1.0f, TargetYaw * -1.0f);
	FRotator CurrentRot = UKismetMathLibrary::MakeRotator(0.0f, m_fPitch, m_fYaw);

	FRotator InterpRot = UKismetMathLibrary::RInterpTo(CurrentRot, TargetRot, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0.0f);

	m_fPitch = InterpRot.Pitch;
	m_fYaw = InterpRot.Yaw;
}

void UPlayerAnimInstance::MovePlayer()
{
	m_fMoveRight = m_player->GetMoveRight();
	m_fMoveForward = m_player->GetMoveForward();
}

void UPlayerAnimInstance::CrounchPlayer()
{
	m_bisCrouch = (m_player->GetPlayterStance() == PlayerStance::Stance_Cruncing);
}

void UPlayerAnimInstance::PlayerStateSet()
{
	m_bisRun = (m_player->GetPlayerGait() == PlayerGait::Gait_Running);
	m_bisJump = m_player->bPressedJump;
	m_bisInAir = m_player->GetMovementComponent()->IsFalling();
	m_bisRifleState = m_player->GetPlayerRifle();
}
