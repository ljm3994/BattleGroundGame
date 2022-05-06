// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BATTLEGROUND_API IItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//아이템과 플레이어 충돌 시 발생하는 이벤트
	virtual void OnInteractiveEnter(class ABattleGroundPlayer* Player) = 0;
	//아이템과 플레이어 충돌 범위 벗어날 시 발생하는 이벤트
	virtual void OnInteractiveExited() = 0;
	//아이템과 플레이어 상호 작용
	virtual void OnInteractive(class ABattleGroundPlayer* Player) = 0;
};
