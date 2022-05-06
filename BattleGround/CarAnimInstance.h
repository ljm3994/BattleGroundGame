// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehicleAnimInstance.h"
#include "CarAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API UCarAnimInstance : public UVehicleAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
		float m_fLeftDoorAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
		float m_fRightDoorAlpha;
public:
	void SetLeftDoorAlpha(float fVal) { m_fLeftDoorAlpha = fVal; }
	void SetRightDoorAlpha(float fVal) { m_fRightDoorAlpha = fVal; }
};
