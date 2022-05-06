// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"
#include "ItemDragWidget.h"
#include "GroundItemWidget.h"
#include "WeaponEquipmentWidget.h"
#include "BattleGroundPlayer.h"
#include "Components/ScrollBox.h"
#include "SlotBaseWidget.h"
#include "DragItemOperation.h"
#include "InventoryWidget.h"
#include "BaseItem.h"
#include "BattleGroundPlayerController.h"

UInventoryItemWidget::UInventoryItemWidget(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
}

void UInventoryItemWidget::NativeConstruct()
{
    Super::NativeConstruct();

    BuildInventory();
}

bool UInventoryItemWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
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
                m_Player->ItemToInventory(widget->ItemRef);
                BuildInventory();
                m_Player->GroundItemAdd();
                m_PlayerController->GetInventoryWidget()->BuildGroundItem();
            }
                break;
            case ESlotCategory::SLOT_EQUIPMENTWEAPON:
            {
                m_Player->ItemToInventory(widget->ItemRef);
                BuildInventory();
                UWeaponEquipmentWidget* EquipmentWidget = Cast< UWeaponEquipmentWidget>(DragItem->ItemData);
                EquipmentWidget->BuildWeapon();
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

void UInventoryItemWidget::BuildInventory()
{
    m_ScrollBox->ClearChildren();
    TArray<FItemData>* Inven = m_Player->GetInventory();
    int index = 0;
    for (auto& data : (*Inven))
    {
        USlotBaseWidget * slot = CreateWidget< USlotBaseWidget>(Cast<ABattleGroundPlayerController>(m_Player->Controller), m_SlotWidget);
        slot->SlotType = ESlotCategory::SLOT_INVENITEM;
        slot->SetData(data);
        slot->m_Player = m_Player;
        slot->m_iIndex = index;
        slot->ItemRef = data.m_ItemClass;

        m_ScrollBox->AddChild(slot);
        index++;
    }
}

void UInventoryItemWidget::RemoveSlot()
{
}
