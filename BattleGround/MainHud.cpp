// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHud.h"
#include "Components/Image.h"
#include "BattleGroundPlayer.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "MiniMapWidget.h"
#include "TimerWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SizeBox.h"
#include "Kismet\KismetSystemLibrary.h"
#include "BaseWeapone.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/TextBlock.h"

UMainHud::UMainHud(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMainHud::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetTimerManager().SetTimer(m_DirectionTimerHandle, FTimerDelegate::CreateLambda([&]() {
		SetDirection();
		}), 0.01f, true);

	GetWorld()->GetTimerManager().SetTimer(m_MakerTimerHandle, FTimerDelegate::CreateLambda([&]() {
		SetMaker();
		}), 0.01f, true);
}

void UMainHud::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	ABaseWeapone* weapon = m_Player->GetCurrentWeapon();
	if (weapon != nullptr)
	{
		float MaxVal = weapon->m_WeaponData.m_HorizonRebound.m_fMaxValue;
		float ClampVal = UKismetMathLibrary::MapRangeClamped(weapon->GetBulletSpeed(), 0.0f, MaxVal, 0.0f, MaxVal * -380.0f);

		m_CrossBImage->SetRenderTranslation(FVector2D(0.0f, ClampVal));
		m_CrossTImage->SetRenderTranslation(FVector2D(0.0f, ClampVal));
		m_CrossLImage->SetRenderTranslation(FVector2D(ClampVal, 0.0f));
		m_CrossRImage->SetRenderTranslation(FVector2D(ClampVal, 0.0f));
	}
}

void UMainHud::NotifyEvent_Implementation(int CurrentTime)
{
	m_NotifySizeBox->SetVisibility(ESlateVisibility::Visible);
	if (CurrentTime == 0)
	{
		m_NotifyText->SetText(FText::FromString("자기장이 줄어들고 있습니다!!!"));
	}
	else
	{
		m_NotifyText->SetText(FText::Format(NSLOCTEXT("Circle", "Circle", "현재 {0}초 남았습니다"), CurrentTime));
	}

	FLatentActionInfo Action;
	Action.CallbackTarget = this;
	Action.UUID = GetUniqueID();
	Action.Linkage = 0;
	Action.ExecutionFunction = "HiddenText";
	UKismetSystemLibrary::Delay(GetWorld(), 4.0f, Action);
}

void UMainHud::BuildHud(ABattleGroundPlayer* player)
{
	m_Player = player;

	m_Camera = m_Player->GetCamer();
	m_PointerMat = m_CompassPointerImage->GetDynamicMaterial();
	m_MakerMat = m_CompassMakerImage->GetDynamicMaterial();

	UMiniMapWidget* minimap = CreateWidget<UMiniMapWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), m_WidgetClass);
	minimap->m_fDemisions = 100000.0;
	minimap->m_fZoom = 5.0;
	minimap->m_Player = m_Player;
	m_MapSizeBox->AddChild(minimap);
	UTimerWidget* TimerWidget = CreateWidget<UTimerWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), m_TimerClass);
	TimerWidget->Owner = this;
	m_TimerSizeBox->AddChild(TimerWidget);
}

void UMainHud::HiddenText()
{
	m_NotifySizeBox->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHud::SetDirection()
{
	FVector Loc = m_Player->GetCamer()->GetComponentLocation();

	m_PointerMat->SetScalarParameterValue("Uoffset", Loc.Z / 360.0f);

}

void UMainHud::SetMaker()
{
	if (m_Player->m_bisMaker)
	{
		m_CompassMakerImage->SetVisibility(ESlateVisibility::Visible);

		float fPoint = 0.0f;

		if (isCheckNavPoint(fPoint))
		{
			m_MakerMat->SetScalarParameterValue("Uoffset", fPoint);
		}
	}
	else
	{
		m_CompassMakerImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool UMainHud::isCheckNavPoint(float& fPoint)
{
	FVector CamLoc = m_Player->GetCamer()->GetComponentLocation();
	FVector RightLoc = m_Player->GetCamer()->GetRightVector();
	FVector ForwardLoc = m_Player->GetCamer()->GetForwardVector();

	FRotator rot = UKismetMathLibrary::FindLookAtRotation(m_Player->NavPoint, CamLoc);
	FVector RotVec = rot.Vector();

	fPoint = UKismetMathLibrary::Dot_VectorVector(RotVec, ForwardLoc);

	return UKismetMathLibrary::Dot_VectorVector(RotVec, RightLoc) > 2.0f;
}
