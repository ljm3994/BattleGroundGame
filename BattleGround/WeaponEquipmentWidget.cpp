// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponEquipmentWidget.h"
#include "BattleGroundPlayer.h"
#include "BaseWeapone.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "BattleGroundPlayerController.h"
#include "InventoryWidget.h"

UWeaponEquipmentWidget::UWeaponEquipmentWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}
void UWeaponEquipmentWidget::NativeConstruct()
{
	BuildWeapon();
}
FReply UWeaponEquipmentWidget::NativeOnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (GetRenderOpacity() > 0.0f)
	{
		m_Player->DropItem(m_Data, Cast<ABaseWeapone>(ItemRef));
		ItemRef->Destroy();
		if (m_Category == m_Player->GetCurrentEquipmentCategory())
		{
			m_Player->SetIsRifle(false);
		}
	}
	return Super::NativeOnMouseButtonUp(MyGeometry, MouseEvent);
}
bool UWeaponEquipmentWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UDragItemOperation* DragItem = Cast< UDragItemOperation>(InOperation);

	if (DragItem != nullptr)
	{
		USlotBaseWidget* widget = Cast< USlotBaseWidget>(DragItem->ItemData);

		if (widget != nullptr)
		{
			switch (widget->SlotType)
			{
			case ESlotCategory::SLOT_GROUNDITEM:
			{
				m_Player->SelectSlotEquipment(m_Category, widget->ItemRef->m_ItemInfo);
				BuildWeapon();
				m_Player->GroundItemAdd();
				m_PlayerController->GetInventoryWidget()->BuildGroundItem();
			}
			break;
			case ESlotCategory::SLOT_INVENITEM:
			{
				int Index = -1;
				bool bisInven = false;
				m_Player->SelectSlotEquipment(m_Category, widget->ItemRef->m_ItemInfo);
				m_Player->GetInvenItem(widget->ItemRef->m_ItemInfo.m_ItemName, Index, bisInven);
				if (bisInven)
				{
					m_Player->RemoveInven(Index);
				}
				BuildWeapon();
				m_PlayerController->GetInventoryWidget()->BuildInventory();
			}
			break;
			default:
				return false;
			}

			return true;
		}
	}
	return false;
}
void UWeaponEquipmentWidget::BuildWeapon()
{
	int Index = 0;
	if (m_Player->GetIsEquipment(m_Category, Index))
	{
		switch (m_Category)
		{
		case EquipmentSlotCategory::Slot_PrimaryWeapon:
			m_ItemNumText->SetText(FText::FromString("1"));
			break;
		case EquipmentSlotCategory::Slot_SecondWeapon:
			m_ItemNumText->SetText(FText::FromString("2"));
			break;
		case EquipmentSlotCategory::Slot_PistolWeapon:
			m_ItemNumText->SetText(FText::FromString("3"));
			break;
		}
		TArray<FEquipmentSlot> Equipment = m_Player->GetEquipmentArray();
		FEquipmentSlot slot = Equipment[Index];
		ABaseWeapone* weapon = Cast<ABaseWeapone>(slot.m_Item);
		ItemRef = slot.m_Item;
		SetRenderOpacity(1.0f);
		m_IconImage->SetBrushFromMaterial(weapon->m_WeaponData.m_Material);
		m_ItemName->SetText(FText::FromName(weapon->m_ItemInfo.m_iItemName));
		switch (weapon->m_WeaponData.m_BulletCategory)
		{
		case EBulletCategory::Bullet_5:
			m_ItemDescription->SetText(FText::FromString("5.56"));
			break;
		case EBulletCategory::Bullet_7:
			m_ItemDescription->SetText(FText::FromString("7.62"));
			break;
		case EBulletCategory::Bullet_4:
			m_ItemDescription->SetText(FText::FromString("4.62"));
			break;
		default:
			break;
		}
		int BulletIndex = 0;
		bool bisItem = false;
		int InvenAmmo = m_Player->GetInvenItem(weapon->m_WeaponData.m_iBulletID, BulletIndex, bisItem);

		m_BulletCountNum->SetText(FText::Format(NSLOCTEXT("Bullet", "Bullet", "{0} | {1}"), weapon->m_WeaponData.m_iAmmoCount, InvenAmmo));
	}
	else
	{
		SetRenderOpacity(0.0f);
	}
}
