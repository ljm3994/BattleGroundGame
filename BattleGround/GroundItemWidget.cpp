// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundItemWidget.h"
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

UGroundItemWidget::UGroundItemWidget(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
}

void UGroundItemWidget::NativeConstruct()
{
    Super::NativeConstruct();

    BuildGround();
}

bool UGroundItemWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    UDragItemOperation* DragItem = Cast< UDragItemOperation>(InOperation);

    if (DragItem != nullptr)
    {
        USlotBaseWidget* widget = Cast< USlotBaseWidget>(DragItem->ItemData);

        if (widget != nullptr)
        {
            switch (widget->SlotType)
            {
            case ESlotCategory::SLOT_INVENITEM:
            {
                m_Player->RemoveInven(DragItem->ItemData->m_iIndex);
                m_Player->DropItem(DragItem->ItemData->m_Data, nullptr);
                m_Player->GroundItemAdd();
                BuildGround();
                m_PlayerController->GetInventoryWidget()->BuildInventory();
            }
            break;
            case ESlotCategory::SLOT_EQUIPMENTWEAPON:
            {
                m_Player->DropItem(DragItem->ItemData->m_Data, nullptr);
                BuildGround();
                m_Player->GroundItemAdd();
                UWeaponEquipmentWidget* EquipmentWidget = Cast< UWeaponEquipmentWidget>(DragItem->ItemData);
                if (EquipmentWidget->m_Category == m_Player->GetCurrentEquipmentCategory())
                {
                    m_Player->SetIsRifle(false);
                }
                EquipmentWidget->BuildWeapon();
            }
            break;
            default:
                return true;
            }

            return true;
        }
    }
    return false;
}

void UGroundItemWidget::BuildGround()
{
    m_ScrollBox->ClearChildren();
    int index = 0;
    for (auto& data : m_Player->GetGroundItem())
    {
        USlotBaseWidget* slot = CreateWidget< USlotBaseWidget>(Cast<ABattleGroundPlayerController>(m_Player->Controller), m_SlotWidget);
        slot->SlotType = ESlotCategory::SLOT_GROUNDITEM;
        slot->SetData(data->m_ItemInfo);
        slot->m_Player = m_Player;
        slot->m_iIndex = index;
        slot->ItemRef = data;

        m_ScrollBox->AddChild(slot);
        index++;
    }
}
