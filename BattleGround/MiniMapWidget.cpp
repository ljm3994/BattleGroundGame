// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapWidget.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Components/Image.h"
#include "BattleGroundPlayer.h"
#include "Materials/MaterialParameterCollection.h"

UMiniMapWidget::UMiniMapWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> MatParam(TEXT("MaterialParameterCollection '/Game/UI/HUD/MiniMap/MiniMapParameter'"));
	m_MatParam = MatParam.Object;
}

void UMiniMapWidget::NativeConstruct()
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), m_MatParam, "Demension", m_fDemisions);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), m_MatParam, "Zoom", m_fZoom);

	m_MapMat = m_MapImage->GetDynamicMaterial();
	m_MapMat->SetTextureParameterValue("MapImage", m_RendtargetImage);
}

void UMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	FVector Location = m_Player->GetActorLocation();
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), m_MatParam, "X", Location.X);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), m_MatParam, "Y", Location.Y);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), m_MatParam, "Zoom", m_fZoom);
}
