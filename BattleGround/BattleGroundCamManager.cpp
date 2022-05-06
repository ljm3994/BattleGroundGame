// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGroundCamManager.h"
#include "CameraAnimInstance.h"
#include "BattleGroundPlayer.h"
#include "CameraInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet\KismetSystemLibrary.h"

ABattleGroundCamManager::ABattleGroundCamManager()
	:Super()
{
	m_CamBehavior = CreateOptionalDefaultSubobject<USkeletalMeshComponent>("CamBehavior");
	if (m_CamBehavior)
	{
		m_CamBehavior->AlwaysLoadOnClient = true;
		m_CamBehavior->AlwaysLoadOnServer = true;
		m_CamBehavior->bOwnerNoSee = false;
		m_CamBehavior->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		m_CamBehavior->bCastDynamicShadow = true;
		m_CamBehavior->bAffectDynamicIndirectLighting = true;
		m_CamBehavior->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		m_CamBehavior->SetupAttachment(GetTransformComponent());
		static FName MeshCollisionProfileName(TEXT("CamBehavior"));
		m_CamBehavior->SetCollisionProfileName(MeshCollisionProfileName);
		m_CamBehavior->SetGenerateOverlapEvents(false);
		m_CamBehavior->SetCanEverAffectNavigation(false);
	}

	m_fThirdFov = 90.0f;
	m_fFirstFov = 90.0f;
}

bool ABattleGroundCamManager::BlueprintUpdateCamera(AActor* CameraTarget, FVector& NewCameraLocation, FRotator& NewCameraRotation, float& NewCameraFOV)
{
	if (CameraTarget->ActorHasTag("Character"))   //캐릭터 태그가 붙은 액터의 카메라만 커스텀 카메라를 사용한다.....
	{
		CustorCameraBehavior(NewCameraLocation, NewCameraRotation, NewCameraFOV);
		return true;
	}
	else
	{
		Super::BlueprintUpdateCamera(CameraTarget, NewCameraLocation, NewCameraRotation, NewCameraFOV);		
		return false;
	}

}

void ABattleGroundCamManager::OnPossess_Implementation(APawn* newPawn)
{
	m_ControllPawn = newPawn;
	UCameraAnimInstance* Anim = Cast< UCameraAnimInstance>(m_CamBehavior->GetAnimInstance());

	if (Anim != nullptr)
	{
		Anim->ControllPawn = newPawn;
	}
}

void ABattleGroundCamManager::CustorCameraBehavior(FVector& Loc, FRotator& Rot, float& FOV)
{
	FTransform transform;
	FVector FPTarget;
	float TPFov;
	float FPFov;
	if (SetCameraParameter(transform, FPTarget, TPFov, FPFov))
	{
		SetCameraRotation();
		CalculateSmoothPivot(transform);
		CalculatePivotPosition();
		CalculateCameraLocation(transform);
		CameraTrace();
		FPLerp(FPTarget, TPFov, FPFov, Loc, Rot, FOV);
	}
}

float ABattleGroundCamManager::GetCameraBehaviorTarget(FName CurveName)
{
	if (IsValid(m_CamBehavior->GetAnimInstance()))
	{
		return m_CamBehavior->GetAnimInstance()->GetCurveValue(CurveName);
	}

	return 0.0f;
}

FVector ABattleGroundCamManager::CalculateAxisIndependent(FVector CurrentLoc, FVector TargetLoc, FRotator CameraRot, FVector LagSpeed)
{
	FRotator CamRotationYaw = FRotator(0.0f, CameraRot.Yaw, 0.0f);
	float DetaTime = GetWorld()->GetDeltaSeconds();
	FVector UnCurrentVec = UKismetMathLibrary::Quat_UnrotateVector(FQuat(CamRotationYaw), CurrentLoc); //벡터를 회전 값을 사용하여 회전시킨 결과를 가져온다
	FVector UnTargetVec = UKismetMathLibrary::Quat_UnrotateVector(FQuat(CamRotationYaw), TargetLoc);
	float XVal = UKismetMathLibrary::FInterpTo(UnCurrentVec.X, UnTargetVec.X, DetaTime, LagSpeed.X);
	float YVal = UKismetMathLibrary::FInterpTo(UnCurrentVec.Y, UnTargetVec.Y, DetaTime, LagSpeed.Y);
	float ZVal = UKismetMathLibrary::FInterpTo(UnCurrentVec.Z, UnTargetVec.Z, DetaTime, LagSpeed.Z);

	FVector InterpVec = UKismetMathLibrary::MakeVector(XVal, YVal, ZVal);
	return UKismetMathLibrary::Quat_RotateVector(FQuat(CamRotationYaw), InterpVec);
}

bool ABattleGroundCamManager::SetCameraParameter(FTransform& PivotTarget, FVector& FPTarget, float& TPFOV, float& FPFOV)
{
	if (m_ControllPawn != nullptr)
	{
		if (m_ControllPawn->GetClass()->ImplementsInterface(UCameraInterface::StaticClass()))
		{
			PivotTarget = ICameraInterface::Execute_GetTPPivotTaget(m_ControllPawn);
			FPTarget = ICameraInterface::Execute_GetFPCameraTarget(m_ControllPawn);
			ICameraInterface::Execute_GetCameraParameter(m_ControllPawn, TPFOV, FPFOV);

			return true;
		}
	}

	return false;
}

void ABattleGroundCamManager::SetCameraRotation()
{
	FRotator rot = GetCameraRotation();
	FRotator CurrentRot = GetOwningPlayerController()->GetControlRotation();
	float fspeed = GetCameraBehaviorTarget("RotationSpeed");
	m_RotTargetCamera = UKismetMathLibrary::RInterpTo(rot, CurrentRot, GetWorld()->GetDeltaSeconds(), fspeed);
}

void ABattleGroundCamManager::CalculateSmoothPivot(FTransform PivotTarget)
{
	FVector vecSpeed = UKismetMathLibrary::MakeVector(GetCameraBehaviorTarget("PivotSpeedX"), GetCameraBehaviorTarget("PivotSpeedY"), GetCameraBehaviorTarget("PivotSpeedZ"));
	FVector vecCal = CalculateAxisIndependent(m_TransSmoothedPivotTarget.GetLocation(), PivotTarget.GetLocation(), m_RotTargetCamera, vecSpeed);
	m_TransSmoothedPivotTarget = UKismetMathLibrary::MakeTransform(vecCal, PivotTarget.GetRotation().Rotator(), FVector(1.0f, 1.0f, 1.0f));
}

void ABattleGroundCamManager::CalculatePivotPosition()
{
	FVector PivotOffsetX = m_TransSmoothedPivotTarget.GetRotation().GetForwardVector() * GetCameraBehaviorTarget("PivotOffsetX");
	FVector PivotOffsetY = m_TransSmoothedPivotTarget.GetRotation().GetForwardVector() * GetCameraBehaviorTarget("PivotOffsetY");
	FVector PivotOffsetZ = m_TransSmoothedPivotTarget.GetRotation().GetForwardVector() * GetCameraBehaviorTarget("PivotOffsetZ");

	m_vecPivotLoc = m_TransSmoothedPivotTarget.GetLocation() + PivotOffsetX + PivotOffsetY + PivotOffsetZ;
}

void ABattleGroundCamManager::CalculateCameraLocation(FTransform PivotTarget)
{
	FVector CameraOffsetX = UKismetMathLibrary::GetForwardVector(m_RotTargetCamera) * GetCameraBehaviorTarget("CameraOffsetX");
	FVector CameraOffsetY = UKismetMathLibrary::GetForwardVector(m_RotTargetCamera) * GetCameraBehaviorTarget("CameraOffsetY");
	FVector CameraOffsetZ = UKismetMathLibrary::GetForwardVector(m_RotTargetCamera) * GetCameraBehaviorTarget("CameraOffsetZ");

	FVector lerpA = m_vecPivotLoc + CameraOffsetX + CameraOffsetY + CameraOffsetZ;

	m_vecTargetCam = UKismetMathLibrary::VLerp(lerpA, PivotTarget.GetLocation() + m_vecDebugView, 1.0f);
}

void ABattleGroundCamManager::CameraTrace()
{
	if (m_ControllPawn != nullptr)
	{
		if (m_ControllPawn->GetClass()->ImplementsInterface(UCameraInterface::StaticClass()))
		{
			FVector Loc = ICameraInterface::Execute_GetTPTraceLocation(m_ControllPawn);
			TArray<AActor*> HitActor;
			FHitResult result;
			UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Loc, m_vecTargetCam, 10.0f, ETraceTypeQuery::TraceTypeQuery1, false, HitActor, EDrawDebugTrace::ForOneFrame, result, true);

			if (result.bBlockingHit && result.Distance != 0)
			{
				m_vecTargetCam = (result.Location - result.TraceEnd) + m_vecTargetCam;
			}
		}
	}
}

void ABattleGroundCamManager::FPLerp(FVector FPTarget, float TPFOV, float FPFOV, FVector& Loc, FRotator& Rot, float& FOV)
{
	FTransform TargetTrans = UKismetMathLibrary::MakeTransform(m_vecTargetCam, m_RotTargetCamera, FVector(1.0f, 1.0f, 1.0f));
	FTransform FPTrans = UKismetMathLibrary::MakeTransform(FPTarget, m_RotTargetCamera, FVector(1.0f, 1.0f, 1.0f));

	FTransform lerpTrans = UKismetMathLibrary::TLerp(TargetTrans, FPTrans, GetCameraBehaviorTarget("Weight_FirstPerson"));
	FTransform TargetLerpTrans = UKismetMathLibrary::TLerp(lerpTrans, TargetTrans, 1.0f);
	Loc = TargetLerpTrans.GetLocation();
	Rot = TargetLerpTrans.GetRotation().Rotator();
	FOV = UKismetMathLibrary::Lerp(TPFOV, FPFOV, GetCameraBehaviorTarget("Weight_FirstPerson"));
}
