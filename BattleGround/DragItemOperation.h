// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemEnum.h"
#include "DragItemOperation.generated.h"

UENUM(BlueprintType)
enum class ESlotCategory : uint8
{
	SLOT_INVENITEM UMETA(DisplayName = "InvenItem"),
	SLOT_GROUNDITEM UMETA(DisplayName = "GroundItem"),
	SLOT_EQUIPMENTWEAPON UMETA(DisplayName = "EquipmentItem"),
};

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API UDragItemOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	FVector2D m_DragOffset;
	ESlotCategory SlotCategory;
	class USlotBaseWidget * ItemData;
};
