// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotBaseWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "ItemHoverWidget.h"
#include "ItemDragWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "BattleGroundPlayer.h"
#include "InventoryWidget.h"
#include "BattleGroundPlayerController.h"
#include "BaseItem.h"
USlotBaseWidget::USlotBaseWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    m_bisDrag = false;
}

void USlotBaseWidget::SetData(FItemData Data)
{
    m_Data = Data;

    m_IconImage->SetBrushFromTexture(Data.m_Icon);
    m_ItemName->SetText(FText::FromString(Data.m_iItemName.ToString()));
    m_ItemDescription->SetText(FText::FromString(""));
    if (Data.m_iItemAmount != 1)
    {
        m_ItemDescription->SetText(FText::AsNumber(Data.m_iItemAmount));
    }

    ItemRef = Data.m_ItemClass;
}

void USlotBaseWidget::UpdatePosition()
{
    float XPos = 0;
    float YPos = 0;
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(XPos, YPos);
    FVector2D Loc = FVector2D(XPos, YPos) + 25.0f;

    m_WidgetHoverRef->SetPositionInViewport(Loc, true);
}

FReply USlotBaseWidget::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    m_VecDragOffset = USlateBlueprintLibrary::AbsoluteToLocal(MyGeometry, MouseEvent.GetScreenSpacePosition());

    return UWidgetBlueprintLibrary::DetectDragIfPressed(MouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

FReply USlotBaseWidget::NativeOnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (m_Data.m_Category != EItemCategory::Item_None)
    {
        bool isTrue = true;

        if (SlotType == ESlotCategory::SLOT_INVENITEM)
        {
            m_Player->RemoveInven(m_iIndex);
            m_Player->DropItem(m_Data, nullptr);
        }
        else if (SlotType == ESlotCategory::SLOT_GROUNDITEM)
        {
            if (!m_Player->ItemToInventory(ItemRef))
            {
                isTrue = false;
            }
            else
            {
                ItemRef->Destroy();
            }
        }

        if (isTrue)
        {
            m_PlayerController->GetInventoryWidget()->BuildInventory();
            m_PlayerController->GetInventoryWidget()->BuildGroundItem();
            m_WidgetHoverRef->RemoveFromParent();
        }

    }
   

    return Super::NativeOnMouseButtonUp(MyGeometry, MouseEvent);
}

void USlotBaseWidget::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    m_WidgetHoverRef = CreateWidget<UItemHoverWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), m_HoverClass);

    if (m_Data.m_Category != EItemCategory::Item_None)
    {
        m_WidgetHoverRef->SetHoverData(m_Data);
        m_WidgetHoverRef->AddToViewport();
        UpdatePosition();

        GetWorld()->GetTimerManager().SetTimer(m_Handler, FTimerDelegate::CreateLambda([&]()
            {
                UpdatePosition();
            }), 0.01f, true);
    }
    
}

void USlotBaseWidget::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
    if (m_WidgetHoverRef != nullptr)
    {
        m_WidgetHoverRef->RemoveFromParent();
        GetWorld()->GetTimerManager().ClearTimer(m_Handler);
    }
    
}

void USlotBaseWidget::NativeOnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, UDragDropOperation*& InOperation)
{
    Super::NativeOnDragDetected(MyGeometry, MouseEvent, InOperation);

    if (InOperation == nullptr)
    {
        m_bisDrag = true;
        UDragItemOperation* DragItem = NewObject< UDragItemOperation>();
        DragItem->m_DragOffset = m_VecDragOffset;
        DragItem->SlotCategory = SlotType;
        DragItem->ItemData = this;

        InOperation = DragItem;

        UItemDragWidget* DragWidget = CreateWidget<UItemDragWidget>(Cast<ABattleGroundPlayerController>(m_Player->Controller), m_DragVisualClass);
        if (DragWidget != nullptr)
        {
            DragWidget->SetDrag(m_Data);
            DragItem->DefaultDragVisual = DragWidget;
        }

    }
}
