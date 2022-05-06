// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHoverWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

UItemHoverWidget::UItemHoverWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UItemHoverWidget::SetHoverData(FItemData Data)
{
	m_IconImage->SetBrushFromTexture(Data.m_Icon);
	m_ItemName->SetText(FText::FromString(Data.m_iItemName.ToString()));
	m_ItemDescription->SetText(FText::FromString(Data.m_iItemName.ToString()));
	m_ItemWeight->SetText(FText::AsNumber(Data.m_fWeight));
}
