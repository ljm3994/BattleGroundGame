// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDragWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

UItemDragWidget::UItemDragWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

UItemDragWidget * UItemDragWidget::SetDrag(FItemData Data)
{
	m_IconImage->SetBrushFromTexture(Data.m_Icon);
	m_ItemName->SetText(FText::FromString(Data.m_iItemName.ToString()));
	m_ItemName->SetText(FText::FromString(""));
	if (Data.m_iItemAmount > 1)
	{
		m_ItemName->SetText(FText::AsNumber(Data.m_iItemAmount));
	}

	return this;
}