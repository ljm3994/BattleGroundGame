// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/Image.h"
#include "WeaponEquipmentWidget.h"
#include "GroundItemWidget.h"
#include "InventoryItemWidget.h"
#include "BattleGroundPlayer.h"
#include "BattleGroundPlayerController.h"
#include "Components/SizeBox.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryWidget::BuildInventory(ABattleGroundPlayer* player, ABattleGroundPlayerController* controller)
{
	m_OwningPlayer = player;
	m_OwningPlayerController = controller;
	m_InventoryList = CreateWidget<UInventoryItemWidget>(Cast<ABattleGroundPlayerController>(m_OwningPlayer->Controller), m_InvenListClass);
	m_InventoryList->m_Player = m_OwningPlayer;
	m_InventoryList->m_PlayerController = m_OwningPlayerController;
	m_InventorySizeBox->AddChild(m_InventoryList);
	m_GroundList = CreateWidget<UGroundItemWidget>(Cast<ABattleGroundPlayerController>(m_OwningPlayer->Controller), m_GroundListClass);
	m_GroundList->m_Player = m_OwningPlayer;
	m_GroundList->m_PlayerController = m_OwningPlayerController;
	m_GroundSizeBox->AddChild(m_GroundList);
	m_FirstWeaponSlot = CreateWidget<UWeaponEquipmentWidget>(Cast<ABattleGroundPlayerController>(m_OwningPlayer->Controller), m_WeaponListClass);
	m_FirstWeaponSlot->m_Player = m_OwningPlayer;
	m_FirstWeaponSlot->m_PlayerController = m_OwningPlayerController;
	m_FirstWeaponSlot->m_Category = EquipmentSlotCategory::Slot_PrimaryWeapon;
	m_OneWeaponSizeBox->AddChild(m_FirstWeaponSlot);
	m_SecondWeaponSlot = CreateWidget<UWeaponEquipmentWidget>(Cast<ABattleGroundPlayerController>(m_OwningPlayer->Controller), m_WeaponListClass);
	m_SecondWeaponSlot->m_Player = m_OwningPlayer;
	m_SecondWeaponSlot->m_PlayerController = m_OwningPlayerController;
	m_SecondWeaponSlot->m_Category = EquipmentSlotCategory::Slot_SecondWeapon;
	m_TwoSizeBox->AddChild(m_SecondWeaponSlot);
	m_ThirdWeaponSlot = CreateWidget<UWeaponEquipmentWidget>(Cast<ABattleGroundPlayerController>(m_OwningPlayer->Controller), m_WeaponListClass);
	m_ThirdWeaponSlot->m_Player = m_OwningPlayer;
	m_ThirdWeaponSlot->m_PlayerController = m_OwningPlayerController;
	m_ThirdWeaponSlot->m_Category = EquipmentSlotCategory::Slot_PistolWeapon;
	m_ThridSizeBox->AddChild(m_ThirdWeaponSlot);
}

EquipmentSlotCategory UInventoryWidget::BuildEquipmentWeapon_Implementation(EquipmentSlotCategory& param)
{
	switch (param)
	{
	case EquipmentSlotCategory::Slot_PrimaryWeapon:
		m_FirstWeaponSlot->BuildWeapon();
		break;
	case EquipmentSlotCategory::Slot_SecondWeapon:
		m_SecondWeaponSlot->BuildWeapon();
		break;
	case EquipmentSlotCategory::Slot_PistolWeapon:
		m_ThirdWeaponSlot->BuildWeapon();
		break;
	}
	return param;
}

void UInventoryWidget::BuildInventory_Implementation()
{
	m_InventoryList->BuildInventory();
}

void UInventoryWidget::BuildGroundItem_Implementation()
{
	m_GroundList->BuildGround();
}
