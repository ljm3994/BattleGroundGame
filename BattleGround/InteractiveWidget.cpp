// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"

UInteractiveWidget::UInteractiveWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), m_Image(nullptr)
{

}

void UInteractiveWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInteractiveWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInteractiveWidget::SetDropIconTex(UTexture2D* IconImage)
{
	m_Image->SetBrushFromTexture(IconImage, true);
}
