// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BattleGroundCamManager.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API ABattleGroundCamManager : public APlayerCameraManager
{
	GENERATED_BODY()
private:
	UPROPERTY(Category = "Mesh", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* m_CamBehavior;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
		class APawn* m_ControllPawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
		FRotator m_RotDebugView;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
		FRotator m_RotTargetCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
		FTransform m_TransSmoothedPivotTarget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
		FVector m_vecPivotLoc;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
		FVector m_vecDebugView;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
		FVector m_vecTargetCam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float m_fThirdFov;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float m_fFirstFov;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		FName m_FPCameraSocketName;
public:
	ABattleGroundCamManager();
	virtual bool BlueprintUpdateCamera(AActor* CameraTarget, FVector& NewCameraLocation, FRotator& NewCameraRotation, float& NewCameraFOV);
	UFUNCTION(BlueprintNativeEvent)
	void OnPossess(class APawn * newPawn);
	virtual void OnPossess_Implementation(class APawn* newPawn);
	UFUNCTION()
	void CustorCameraBehavior(FVector& Loc, FRotator& Rot, float& FOV);
	UFUNCTION()
	float GetCameraBehaviorTarget(FName CurveName);
	UFUNCTION()
	FVector CalculateAxisIndependent(FVector CurrentLoc, FVector TargetLoc, FRotator CameraRot, FVector LagSpeed);
	bool SetCameraParameter(FTransform& PivotTarget, FVector& FPTarget, float& TPFOV, float& FPFOV);
	void SetCameraRotation();
	void CalculateSmoothPivot(FTransform PivotTarget);
	void CalculatePivotPosition();
	void CalculateCameraLocation(FTransform PivotTarget);
	void CameraTrace();
	void FPLerp(FVector FPTarget, float TPFOV, float FPFOV, FVector& Loc, FRotator& Rot, float& FOV);
};
