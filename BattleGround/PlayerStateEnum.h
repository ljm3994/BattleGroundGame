// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemEnum.h"
#include "PlayerStateEnum.generated.h"

class ABaseItem;

/**
 *
 */
UENUM(BlueprintType)
enum class PlayerStance : uint8
{
	Stance_Standing UMETA(DisplayName = "Standing"),
	Stance_Cruncing UMETA(DisplayName = "Crunc"),
	Stance_Prone UMETA(DisplayName = "Prone")
};

UENUM(BlueprintType)
enum class PlayerMoveState : uint8
{
	State_Ground  UMETA(DisplayName = "Ground"),
	State_Air  UMETA(DisplayName = "Air"),
	State_Water  UMETA(DisplayName = "Water"),
};

UENUM(BlueprintType)
enum class PlayerViewMode : uint8
{
	View_First  UMETA(DisplayName = "FirstPerson"),
	View_Third  UMETA(DisplayName = "ThordPerson"),
};

UENUM(BlueprintType)
enum class PlayerGait : uint8
{
	Gait_Walking  UMETA(DisplayName = "Walking"),
	Gait_Running UMETA(DisplayName = "Running"),
	Gait_Sprinting UMETA(DisplayName = "Sprinting"),
};

UENUM(BlueprintType)
enum class EquipmentSlotCategory : uint8
{
	Slot_PrimaryWeapon  UMETA(DisplayName = "PrimaryWeapon"),
	Slot_SecondWeapon  UMETA(DisplayName = "SecondWeapon"),
	Slot_PistolWeapon  UMETA(DisplayName = "PistolWeapon"),
	Slot_Meel  UMETA(DisplayName = "Meel"),
	Slot_Grande  UMETA(DisplayName = "Grande"),
	Slot_Head  UMETA(DisplayName = "Head"),
	Slot_Cloth  UMETA(DisplayName = "Cloth"),
	Slot_Armor  UMETA(DisplayName = "Armor"),
	Slot_Pants  UMETA(DisplayName = "Pants"),
	Slot_Glove  UMETA(DisplayName = "Glove"),
	Slot_BackPack  UMETA(DisplayName = "BackPack"),
};

USTRUCT(BlueprintType)
struct FEquipmentSlot : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EquipmentSlotCategory m_SlotCategory;             //���� ī����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName m_ItemName;                            //������ ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ABaseItem* m_Item;                      //������ Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bisSlotOpen;              //������ �����ִ��� �����ִ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName m_strSocketName;              //���� ���� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemCategory m_ItemCategory;              //������ ����
};
